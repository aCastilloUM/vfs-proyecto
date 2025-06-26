#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vfs.h"

#define MAX_ENTRIES 1024

struct entry_info {
    uint32_t inode_nbr;
    char name[FILENAME_MAX_LEN];
    struct inode inode;
};

// Función de comparación para qsort
int compare_entries(const void *a, const void *b) {
    const struct entry_info *ea = (const struct entry_info *)a;
    const struct entry_info *eb = (const struct entry_info *)b;
    return strncmp(ea->name, eb->name, FILENAME_MAX_LEN);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Uso: %s disco.vfs\n", argv[0]);
        return 1;
    }

    const char *vfs_path = argv[1];
    struct inode root_inode;

    if (read_inode(vfs_path, ROOTDIR_INODE, &root_inode) != 0) {
        fprintf(stderr, "No se pudo leer el inodo raíz\n");
        return 1;
    }

    struct entry_info entries[MAX_ENTRIES];
    int entry_count = 0;

    for (int i = 0; i < root_inode.blocks; i++) {
        int block_num = get_block_number_at(vfs_path, &root_inode, i);
        if (block_num <= 0) {
            fprintf(stderr, "Bloque de directorio inválido\n");
            return 1;
        }

        uint8_t data_buf[BLOCK_SIZE];
        if (read_block(vfs_path, block_num, data_buf) != 0) {
            fprintf(stderr, "No se pudo leer bloque del directorio\n");
            return 1;
        }

        struct dir_entry *dir_entries = (struct dir_entry *)data_buf;

        for (size_t j = 0; j < DIR_ENTRIES_PER_BLOCK; j++) {
            if (dir_entries[j].inode == 0) continue;

            // Llenar información de la entrada
            struct entry_info ei;
            ei.inode_nbr = dir_entries[j].inode;
            strncpy(ei.name, dir_entries[j].name, FILENAME_MAX_LEN);
            if (read_inode(vfs_path, ei.inode_nbr, &ei.inode) != 0) {
                fprintf(stderr, "No se pudo leer el inodo %u\n", ei.inode_nbr);
                continue;
            }

            entries[entry_count++] = ei;

            if (entry_count >= MAX_ENTRIES) {
                fprintf(stderr, "Demasiadas entradas de directorio\n");
                break;
            }
        }
    }

    // Ordenar por nombre
    qsort(entries, entry_count, sizeof(struct entry_info), compare_entries);

    // Mostrar resultados
    for (int i = 0; i < entry_count; i++) {
        print_inode(&entries[i].inode, entries[i].inode_nbr, entries[i].name);
    }

    return 0;
}
