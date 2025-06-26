#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vfs.h"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Uso: %s disco.vfs nombre_archivo\n", argv[0]);
        return 1;
    }

    const char *image_path = argv[1];
    const char *filename = argv[2];

    if (!name_is_valid(filename)) {
        fprintf(stderr, "Nombre de archivo inválido\n");
        return 1;
    }

    int inode_number = dir_lookup(image_path, filename);
    if (inode_number <= 0) {
        fprintf(stderr, "Archivo '%s' no encontrado\n", filename);
        return 1;
    }

    struct inode in;
    if (read_inode(image_path, inode_number, &in) != 0) {
        fprintf(stderr, "No se pudo leer el inodo del archivo '%s'\n", filename);
        return 1;
    }

    // Truncar datos (libera bloques si hay)
    if (inode_trunc_data(image_path, &in) != 0) {
        fprintf(stderr, "Error al liberar bloques de datos del archivo '%s'\n", filename);
        return 1;
    }

    // Liberar inodo
    if (free_inode(image_path, inode_number) != 0) {
        fprintf(stderr, "Error al liberar el inodo del archivo '%s'\n", filename);
        return 1;
    }

    // Eliminar del directorio
    if (remove_dir_entry(image_path, filename) != 0) {
        fprintf(stderr, "Error al eliminar la entrada del directorio '%s'\n", filename);
        return 1;
    }

    printf("Archivo '%s' eliminado exitosamente\n", filename);
    return 0;
}
