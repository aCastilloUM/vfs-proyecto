#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "vfs.h"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Uso: %s <imagen.vfs> <nombre_archivo>\n", argv[0]);
        return 1;
    }

    const char *vfs_path = argv[1];
    const char *filename = argv[2];

    // Validar nombre de archivo
    if (!name_is_valid(filename)) {
        fprintf(stderr, "Nombre de archivo inválido o muy largo\n");
        return 1;
    }

    // Verificar si el archivo ya existe en el directorio raíz
    int lookup_res = dir_lookup(vfs_path, filename);
    if (lookup_res < 0) {
        fprintf(stderr, "Error al acceder al directorio raíz\n");
        return 1;
    } else if (lookup_res > 0) {
        fprintf(stderr, "El archivo '%s' ya existe\n", filename);
        return 1;
    }

    // Crear un inodo vacío con permisos por defecto
    int new_inode = create_empty_file_in_free_inode(vfs_path, DEFAULT_PERM);
    if (new_inode <= 0) {
        fprintf(stderr, "No se pudo crear un nuevo inodo\n");
        return 1;
    }

    // Agregar entrada al directorio raíz
    if (add_dir_entry(vfs_path, filename, new_inode) < 0) {
        fprintf(stderr, "No se pudo agregar la entrada '%s' al directorio raíz\n", filename);
        // Si falla, liberamos el inodo creado
        free_inode(vfs_path, new_inode);
        return 1;
    }

    printf("Archivo '%s' creado exitosamente con inodo %d\n", filename, new_inode);
    return 0;
}
