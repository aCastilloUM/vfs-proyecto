#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "vfs.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <imagen.vfs>\n", argv[0]);
        return 1;
    }

    const char *image_path = argv[1];
    struct inode root_inode;

    // Leer el inodo del directorio raíz
    if (read_inode(image_path, ROOTDIR_INODE, &root_inode) != 0) {
        fprintf(stderr, "Error: no se pudo leer el inodo del directorio raíz\n");
        return 1;
    }

    // Recorrer los bloques de datos del directorio raíz
    for (uint16_t i = 0; i < root_inode.blocks; i++) {
        int block_num = get_block_number_at(image_path, &root_inode, i);
        if (block_num <= 0) {
            fprintf(stderr, "Error: no se pudo obtener el bloque número %d del directorio raíz\n", i);
            return 1;
        }

        uint8_t data_buf[BLOCK_SIZE];
        if (read_block(image_path, block_num, data_buf) != 0) {
            fprintf(stderr, "Error al leer el bloque de datos del directorio raíz\n");
            return 1;
        }

        struct dir_entry *entries = (struct dir_entry *)data_buf;

        // Recorremos las entradas del bloque
        for (size_t j = 0; j < DIR_ENTRIES_PER_BLOCK; j++) {
            if (entries[j].inode == 0) continue;

            struct inode entry_inode;
            if (read_inode(image_path, entries[j].inode, &entry_inode) != 0) {
                fprintf(stderr, "Error al leer el inodo %u\n", entries[j].inode);
                continue;
            }

            print_inode(&entry_inode, entries[j].inode, entries[j].name);
        }
    }

    return 0;
}
