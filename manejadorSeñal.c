//Este código maneja señales mediante signal()

#include <stdio.h>
#include <signal.h>
#include <unistd.h>

// Manejador para la señal SIGINT
void manejador_SIGINT(int signum) {
    printf("¡Señal SIGINT recibida! Terminando el programa...\n");
    _exit(0);  // Terminamos el programa de manera controlada
}

int main() {
    // Configuramos el manejador para la señal SIGINT
    signal(SIGINT, manejador_SIGINT);

    printf("Esperando señal SIGINT (presiona Ctrl+C)...\n");

    // El programa entra en un bucle infinito esperando señales
    while (1) {
        printf("working\n");// El proceso está esperando por señales, puedes hacer cualquier otra cosa aquí.
        sleep(1);  // Simulamos que el programa está trabajando
    }
    
    return 0;
}
