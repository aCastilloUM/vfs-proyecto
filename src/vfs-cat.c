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

    // Validación del nombre de archivo
    if (!name_is_valid(filename)) {
        fprintf(stderr, "Nombre de archivo inválido\n");
        return 1;
    }

    // Buscar archivo en directorio raíz
    int inode_number = dir_lookup(image_path, filename);
    if (inode_number <= 0) {
        fprintf(stderr, "Archivo '%s' no encontrado\n", filename);
        return 1;
    }

    // Leer el inodo del archivo
    struct inode in;
    if (read_inode(image_path, inode_number, &in) != 0) {
        fprintf(stderr, "No se pudo leer el inodo del archivo '%s'\n", filename);
        return 1;
    }

    // Si el archivo está vacío, imprimir nada pero no fallar
    if (in.size == 0) {
        return 0;
    }

    // Reservar buffer
    void *data_buf = malloc(in.size);
    if (!data_buf) {
        fprintf(stderr, "No hay memoria suficiente\n");
        return 1;
    }

    // Leer los datos
    int bytes_read = inode_read_data(image_path, inode_number, data_buf, in.size, 0);
    if (bytes_read < 0) {
        fprintf(stderr, "Error al leer los datos del archivo '%s'\n", filename);
        free(data_buf);
        return 1;
    }

    // Escribir contenido a stdout
    fwrite(data_buf, 1, bytes_read, stdout);
    free(data_buf);
    return 0;
}
