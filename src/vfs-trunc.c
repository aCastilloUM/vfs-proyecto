#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vfs.h"

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Uso: %s disco.vfs archivo1 [archivo2 ...]\n", argv[0]);
        return 1;
    }

    const char *image_path = argv[1];

    for (int i = 2; i < argc; ++i) {
        const char *filename = argv[i];

        if (!name_is_valid(filename)) {
            fprintf(stderr, "Nombre inválido: '%s'\n", filename);
            continue;
        }

        int inode_number = dir_lookup(image_path, filename);
        if (inode_number <= 0) {
            fprintf(stderr, "Archivo '%s' no encontrado\n", filename);
            continue;
        }

        struct inode in;
        if (read_inode(image_path, inode_number, &in) != 0) {
            fprintf(stderr, "No se pudo leer el inodo de '%s'\n", filename);
            continue;
        }

        if ((in.mode & INODE_MODE_FILE) != INODE_MODE_FILE) {
            fprintf(stderr, "'%s' no es un archivo regular\n", filename);
            continue;
        }

        if (inode_trunc_data(image_path, &in) != 0) {
            fprintf(stderr, "Error al truncar los datos de '%s'\n", filename);
            continue;
        }

        // Guardar el inodo actualizado (sin bloques y tamaño 0)
        if (write_inode(image_path, inode_number, &in) != 0) {
            fprintf(stderr, "Error al escribir inodo truncado para '%s'\n", filename);
            continue;
        }

        printf("Archivo '%s' truncado correctamente\n", filename);
    }

    return 0;
}
