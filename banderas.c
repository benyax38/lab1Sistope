// Este código acepta argumentos por medio de banderas mediante la función getopt()
// Utilizar esta entrada desde la terminal: ./mi_programa -a -b hola archivo1 archivo2


#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    int opt;

    // Opciones válidas: a, b (requiere argumento)
    while ((opt = getopt(argc, argv, "ab:")) != -1) {
        switch (opt) {
            case 'a':
                printf("Opción -a\n");
                break;
            case 'b':
                printf("Opción -b con argumento: %s\n", optarg);
                break;
            case '?':
                // getopt imprime un mensaje por defecto si hay un error
                printf("Opción desconocida o falta argumento\n");
                break;
        }
    }

    // Argumentos que no son opciones (resto)
    for (int i = optind; i < argc; i++) {
        printf("Argumento extra: %s\n", argv[i]);
    }

    return 0;
}
