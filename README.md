# Integrantes: Agustin Castillo, Martín Estefanell

# Obligatorio 2025 - Sistemas Operativos

Este proyecto implementa un **sistema de archivos virtual (VFS)** en C, utilizando bloques de 1024 bytes, con operaciones básicas de manejo de archivos y directorios dentro de una imagen de disco (`disco.vfs`).

---

## 📦 Comandos implementados

### ✅ `vfs-mkfs`
Crea una nueva imagen de disco virtual (`disco.vfs`) con tamaño y cantidad de inodos indicados. Inicializa el superbloque, tabla de inodos, bitmap y el directorio raíz.

### ✅ `vfs-touch`
Crea un nuevo archivo vacío en el directorio raíz del VFS.

### ✅ `vfs-copy`
Copia un archivo del sistema real al VFS, escribiéndolo bloque por bloque.

### ✅ `vfs-cat`
Muestra en pantalla el contenido de un archivo dentro del VFS.

### ✅ `vfs-rm`
Elimina un archivo del VFS, liberando inodo y bloques.

### ✅ `vfs-ls`
Lista los archivos y directorios del directorio raíz del VFS mostrando:
- Nombre
- Número de inodo
- Usuario y grupo
- Permisos
- Tamaño
- Cantidad de bloques
- Fechas de creación, modificación y acceso

### ✅ `vfs-lsort`
Lista los archivos del directorio raíz del VFS de forma ordenada alfabéticamente.

### ✅ `vfs-info`
Muestra información del superbloque y el estado general del VFS:
- Tamaño de bloque
- Cantidad de bloques
- Cantidad de inodos
- Bloques e inodos libres
- Bitmap visual del estado de bloques

### ✅ `vfs-trunc`
Trunca archivos dentro del VFS, dejándolos en tamaño 0 y liberando bloques.

### ✅ `vfs-mkdir`
Crea un nuevo directorio en el VFS e inicializa con `.` y `..`.

### ✅ `vfs-lsdir`
Lista el contenido de un subdirectorio específico del VFS.

### ✅ `vfs-rename`
Renombra archivos o directorios dentro del VFS.

---

## ⚙️ Compilación

Para compilar todos los comandos:
```bash
make
```

Para limpiar los binarios:
```bash
make clean
```

---

## 🛠️ Uso

Crear un disco:
```bash
./vfs-mkfs disco.vfs 512 64
```

Crear un archivo:
```bash
./vfs-touch disco.vfs archivo.txt
```

Copiar archivo:
```bash
./vfs-copy disco.vfs local.txt archivoVFS.txt
```

Ver contenido:
```bash
./vfs-cat disco.vfs archivoVFS.txt
```

Listar archivos:
```bash
./vfs-ls disco.vfs
```

Eliminar archivo:
```bash
./vfs-rm disco.vfs archivoVFS.txt
```

Renombrar archivo:
```bash
./vfs-rename disco.vfs viejo.txt nuevo.txt
```

Truncar archivo:
```bash
./vfs-trunc disco.vfs archivoVFS.txt
```

Crear directorio:
```bash
./vfs-mkdir disco.vfs carpeta
```

Listar subdirectorio:
```bash
./vfs-lsdir disco.vfs carpeta
```

---

## 📚 Notas

- Cada bloque del VFS tiene 1024 bytes.
- Actualmente las operaciones se realizan sobre el directorio raíz.
- Este proyecto es utilizado para aprender sistemas de archivos, inodos, superbloques y manejo de estructuras en C.

---

Fin del Obligatorio 2025 - UM Sistemas Operativos.
