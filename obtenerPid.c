//Este código obtiene los identificadores de un proceso padre y un proceso hijo

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    pid_t pid = fork();  // Crear un proceso hijo

    if (pid == 0) {
        // Este es el proceso hijo
        printf("Soy el proceso hijo.\n");
        printf("Mi PID es: %d\n", getpid());
        printf("El PID de mi proceso padre es: %d\n", getppid());
        exit(0);  // Termina el proceso hijo con estado 0
    } else {
        // Este es el proceso padre
        printf("Soy el proceso padre.\n");
        printf("Mi PID es: %d\n", getpid());
        printf("El PID de mi proceso hijo es: %d\n", pid);
        wait(NULL);  // Espera a que el hijo termine
        printf("El proceso hijo ha terminado.\n");
    }

    return 0;
}
