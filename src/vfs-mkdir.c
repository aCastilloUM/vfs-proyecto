#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "vfs.h"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Uso: %s disco.vfs nombre_directorio\n", argv[0]);
        return 1;
    }

    const char *image_path = argv[1];
    const char *dirname = argv[2];

    if (!name_is_valid(dirname)) {
        fprintf(stderr, "Nombre de directorio inválido\n");
        return 1;
    }

    // Verificar si ya existe
    if (dir_lookup(image_path, dirname) > 0) {
        fprintf(stderr, "El archivo o directorio '%s' ya existe\n", dirname);
        return 1;
    }

    // Crear inodo de tipo directorio
    int inode_number = create_empty_file_in_free_inode(image_path, DEFAULT_PERM | INODE_MODE_DIR);
    if (inode_number <= 0) {
        fprintf(stderr, "No se pudo crear el inodo para el directorio\n");
        return 1;
    }

    // Reservar un bloque para el contenido del directorio
    int data_block = bitmap_set_first_free(image_path);
    if (data_block < 0) {
        fprintf(stderr, "No se pudo reservar un bloque de datos\n");
        free_inode(image_path, inode_number);
        return 1;
    }

    // Leer el inodo recién creado
    struct inode new_dir_inode;
    if (read_inode(image_path, inode_number, &new_dir_inode) != 0) {
        fprintf(stderr, "Error al leer el nuevo inodo\n");
        return 1;
    }

    // Inicializar bloque con '.' y '..'
    struct dir_entry entries[DIR_ENTRIES_PER_BLOCK] = {0};

    entries[0].inode = inode_number;
    strncpy(entries[0].name, ".", FILENAME_MAX_LEN);

    entries[1].inode = ROOTDIR_INODE;
    strncpy(entries[1].name, "..", FILENAME_MAX_LEN);

    if (write_block(image_path, data_block, entries) != 0) {
        fprintf(stderr, "Error al escribir el bloque de datos del nuevo directorio\n");
        return 1;
    }

    // Actualizar el inodo
    new_dir_inode.direct[0] = data_block;
    new_dir_inode.blocks = 1;
    new_dir_inode.size = sizeof(struct dir_entry) * 2;
    new_dir_inode.ctime = new_dir_inode.mtime = new_dir_inode.atime = time(NULL);

    if (write_inode(image_path, inode_number, &new_dir_inode) != 0) {
        fprintf(stderr, "Error al actualizar el inodo del nuevo directorio\n");
        return 1;
    }

    // Agregar al directorio raíz
    if (add_dir_entry(image_path, dirname, inode_number) < 0) {
        fprintf(stderr, "No se pudo agregar la entrada en el directorio raíz\n");
        free_inode(image_path, inode_number);
        return 1;
    }

    printf("Directorio '%s' creado exitosamente\n", dirname);
    return 0;
}
