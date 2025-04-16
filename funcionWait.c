//Este código crea un proceso hijo y el padre espera a que el proceso hijo finalice

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
        exit(0);  // Termina el proceso hijo con estado 0
    } else {
        // Este es el proceso padre
        wait(NULL);  // Espera a que el hijo termine
        printf("El proceso hijo ha terminado.\n");
    }

    return 0;
}
