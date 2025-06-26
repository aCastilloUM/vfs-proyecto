#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vfs.h"

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Uso: %s disco.vfs archivo_origen archivo_destino\n", argv[0]);
        return 1;
    }

    const char *image_path = argv[1];
    const char *source_name = argv[2];
    const char *target_name = argv[3];

    // Validaciones básicas
    if (!name_is_valid(source_name) || !name_is_valid(target_name)) {
        fprintf(stderr, "Nombre de archivo inválido\n");
        return 1;
    }

    if (strcmp(source_name, target_name) == 0) {
        fprintf(stderr, "El nombre de destino es igual al de origen\n");
        return 1;
    }

    // Verificar existencia del archivo origen
    int inode_n = dir_lookup(image_path, source_name);
    if (inode_n <= 0) {
        fprintf(stderr, "Archivo '%s' no encontrado\n", source_name);
        return 1;
    }

    // Verificar que el destino no exista
    int dest_inode = dir_lookup(image_path, target_name);
    if (dest_inode > 0) {
        fprintf(stderr, "Ya existe un archivo con el nombre '%s'\n", target_name);
        return 1;
    }

    // Buscar y renombrar en la entrada del directorio
    struct inode root_inode;
    if (read_inode(image_path, ROOTDIR_INODE, &root_inode) != 0) {
        fprintf(stderr, "No se pudo leer el inodo raíz\n");
        return 1;
    }

    for (int i = 0; i < root_inode.blocks; i++) {
        int block_num = get_block_number_at(image_path, &root_inode, i);
        if (block_num <= 0) {
            fprintf(stderr, "Error inesperado al buscar bloque %d del directorio raíz\n", i);
            return 1;
        }

        uint8_t data_buf[BLOCK_SIZE];
        if (read_block(image_path, block_num, data_buf) != 0)
            return 1;

        struct dir_entry *entries = (struct dir_entry *)data_buf;

        for (int j = 0; j < DIR_ENTRIES_PER_BLOCK; j++) {
            if (entries[j].inode == inode_n &&
                strncmp(entries[j].name, source_name, FILENAME_MAX_LEN) == 0) {

                strncpy(entries[j].name, target_name, FILENAME_MAX_LEN);
                if (write_block(image_path, block_num, data_buf) != 0) {
                    fprintf(stderr, "No se pudo escribir el bloque de directorio actualizado\n");
                    return 1;
                }

                printf("Archivo '%s' renombrado a '%s'\n", source_name, target_name);
                return 0;
            }
        }
    }

    fprintf(stderr, "No se encontró la entrada de directorio correspondiente\n");
    return 1;
}
