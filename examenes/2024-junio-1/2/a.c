#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

#define THRESHOLD 300 * 1024 // 300 KB en bytes
#define FOLDER_01 "Folder_01"
#define FOLDER_02 "Folder_02"

void process_file(const char *filename) {
    struct stat file_stat;
    char destination_path[256];

    // Obtener información del archivo
    if (stat(filename, &file_stat) == -1) {
        perror("Error obteniendo información del archivo");
        return;
    }

    // Determinar el directorio de destino
    const char *target_folder = (file_stat.st_size > THRESHOLD) ? FOLDER_01 : FOLDER_02;
    snprintf(destination_path, sizeof(destination_path), "%s/%s", target_folder, filename);

    // Crear enlace rígido
    if (link(filename, destination_path) == 0) {
        printf("Link to %s created successfully in %s\n", filename, target_folder);
    } else {
        perror("Error creando enlace");
    }
}

int main() {
    DIR *dir;
    struct dirent *entry;

    // Abrir el directorio actual
    dir = opendir(".");
    if (dir == NULL) {
        perror("Error abriendo el directorio actual");
        return EXIT_FAILURE;
    }

    // Crear directorios Folder_01 y Folder_02 si no existen
    mkdir(FOLDER_01, 0755);
    mkdir(FOLDER_02, 0755);

    // Recorrer los archivos del directorio
    while ((entry = readdir(dir)) != NULL) {
        // Filtrar solo archivos con extensión .png
        if (strstr(entry->d_name, ".png") != NULL) {
            process_file(entry->d_name);
        }
    }

    closedir(dir);
    return EXIT_SUCCESS;
}
