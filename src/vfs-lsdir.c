#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vfs.h"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Uso: %s disco.vfs nombre_directorio\n", argv[0]);
        return 1;
    }

    const char *image_path = argv[1];
    const char *dirname = argv[2];

    if (!name_is_valid(dirname)) {
        fprintf(stderr, "Nombre inválido\n");
        return 1;
    }

    int inode_number = dir_lookup(image_path, dirname);
    if (inode_number <= 0) {
        fprintf(stderr, "Directorio '%s' no encontrado\n", dirname);
        return 1;
    }

    struct inode dir_inode;
    if (read_inode(image_path, inode_number, &dir_inode) != 0) {
        fprintf(stderr, "Error al leer el inodo del directorio '%s'\n", dirname);
        return 1;
    }

    if ((dir_inode.mode & INODE_MODE_DIR) == 0) {
        fprintf(stderr, "'%s' no es un directorio\n", dirname);
        return 1;
    }

    for (int i = 0; i < dir_inode.blocks; i++) {
        int block_num = get_block_number_at(image_path, &dir_inode, i);
        if (block_num < 0) continue;

        uint8_t buffer[BLOCK_SIZE];
        if (read_block(image_path, block_num, buffer) != 0) continue;

        struct dir_entry *entries = (struct dir_entry *)buffer;
        for (int j = 0; j < DIR_ENTRIES_PER_BLOCK; j++) {
            if (entries[j].inode != 0) {
                struct inode file_inode;
                if (read_inode(image_path, entries[j].inode, &file_inode) == 0) {
                    print_inode(&file_inode, entries[j].inode, entries[j].name);
                }
            }
        }
    }

    return 0;
}
