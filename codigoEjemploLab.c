#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

volatile sig_atomic_t token_recibido = 0; //Permite inicializar el token de forma segura

/*
 * Manejador de señales que procesa la señal SIGUSR1.
 * Se utiliza SA_SIGINFO para acceder a la información adicional enviada con la señal.
 */
void manejador(int sig, siginfo_t *si, void *context) { //firma
    // Extrae el token enviado a través del campo sival_int de la unión sigval.
    int token = si->si_value.sival_int;
    printf("Proceso %d recibió el token: %d\n", getpid(), token);
    token_recibido = 1; // Indica que la señal ha sido recibida.
}

int main() {
    //definición de estructuras y variables necesarias para el manejo de señales
    struct sigaction sa; //estructura definida para el manejo de señales
    sigset_t mask, oldmask; //representan conjuntos de señales

    // Configuración del manejador para SIGUSR1.
    sa.sa_flags = SA_SIGINFO; // Permite acceder a información extendida.
    //Con lo anterior, el manejador sabe que se envió una señal, quien la envió,
    //su PID y datos extra con sigqueue()
    sa.sa_sigaction = manejador; // Asigna el manejador definido.
    //Allí se asigna la función que actuará como manejador de señales
    //Manejador es el nombre de tu función que se ejecutará cuando llegue la señal SIGUSR1
    sigemptyset(&sa.sa_mask); //Indica que no se bloquearán señales mientras se ejecute
    //el manejador

    // Configuración de la acción de SIGUSR1.
    if (sigaction(SIGUSR1, &sa, NULL) == -1) {
        //Cuando llegue la señal SIGUSR1, usa la configuración que está en sa (función
        //manejador, flags, señales bloqueadas, etc)
        perror("sigaction"); //si falla sigaction retorna -1 y muestra este mensaje
        exit(EXIT_FAILURE); //el programa termina con un código de salida que indica el
        //fallo
    }

    // Bloquea SIGUSR1 para evitar su entrega prematura.
    //Este bloque permite tener control para decidir cuando quieres recibir la señal
    //SIGUSR1 queda bloqueada temporalmente, por lo que si se envía mientras está
    //bloqueada, se queda en espera hasta que se desbloquee.
    sigemptyset(&mask); //inicializa el conjunto de señales en vacío. Es necesario antes
    //de agregar señales al conjunto (como SIGUSR1)
    sigaddset(&mask, SIGUSR1); //Agrega la señal SIGUSR1 al conjunto de señales mask
    //mask representa al conjunto de señales que queremos bloquear
    if (sigprocmask(SIG_BLOCK, &mask, &oldmask) < 0) {
        //Esta es la función que realmente bloquea la señal
        //SIG_BLOCK indica que queremos bloquear las señales del conjunto mask
        //mask es el conjunto de señales a bloquear (en este caso, sólo SIGUSR1)
        //oldmask guarda el conjunto de señales que ya estaban bloqueadas antes
        perror("sigprocmask"); //si sigprocmask falla, se imprime este mensaje de error
        exit(EXIT_FAILURE); //el programa termina
    }

    pid_t pid = fork(); //se crea un nuevo proceso hijo
    //fork() retorna dos valores:
    //en el proceso padre devuelve el PID del hijo
    //en el proceso hijo devuelve 0
    if (pid == -1) {
        perror("fork"); //si el pid del proceso es -1 significa que la falló
        //la creación del proceso hijo
        exit(EXIT_FAILURE); //se finaliza el programa
    } else if (pid == 0) { //se entra en este bloque si estamos en el proceso hijo
        // Proceso hijo: espera la señal de manera segura usando sigsuspend.
        while (!token_recibido) { //token es una variable global que cambiará a true
            //cuando reciba la señal (mientras no reciba el token seguirá esperando)
            sigsuspend(&oldmask);
            // sigsuspend reemplaza temporalmente la máscara actual por 'oldmask',
            // permitiendo la recepción de SIGUSR1.
        }
        exit(0); //una vez que el token fue recibido, el proceso hijo termina
    } else {
        // Proceso padre: prepara y envía la señal con el token.
        union sigval value; //permite enviar datos adicionales junto con una señal
        //usando sigqueue()
        value.sival_int = 42; // Asigna el token para enviarlo junto
        // a la señal (valor de ejemplo).

        if (sigqueue(pid, SIGUSR1, value) == -1) {
            //la función sigqueue es como la función kill() con la diferencia
            //de que permite enviar información extra
            //Se envía la señal SIGUSR1 al proceso hijo
            perror("sigqueue"); //envía mensaje en caso de error
            exit(EXIT_FAILURE); //finaliza el programa si no se envió la señal correctamente
        }

        wait(NULL); // El proceso padre espera a que el proceso hijo termine.
        //Esto evita que el hijo quede como proceso zombie
    }

    return 0;
}
