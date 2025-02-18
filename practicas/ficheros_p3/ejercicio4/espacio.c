#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
/* Forward declaration */
int get_size_dir(char *fname, size_t *blocks);
/* Gets in the blocks buffer the size of file fname using lstat. If fname
is a
* directory get_size_dir is called to add the size of its contents.
*/
int get_size(char *fname, size_t *blocks)
{
    struct stat sb;
    if (lstat(fname, &sb) == -1)
    {
        perror("Error al llamar a lstat");
        return -1;
    }
    switch (sb.st_mode & __S_IFMT)
    {
    case __S_IFDIR:
        if (get_size_dir(fname, blocks) == -1)
        {
            perror("Error en get_size_dir");
            return -1;
        }
        break;
    case __S_IFREG:
        *blocks += sb.st_blocks;
        break;
    default:
        break;
    }
    return 0;
}
/* Gets the total number of blocks occupied by all the files in a
directory. If
* a contained file is a directory a recursive call to get_size_dir is
* performed. Entries . and .. are conveniently ignored.
*/
int get_size_dir(char *dname, size_t *blocks)
{
    DIR *dir;
    struct dirent *next_dir;
    struct stat sb;
    dir = opendir(dname);
    if (!dir)
    {
        perror("No se ha podido abrir el archivo");
        return -1;
    }
    while ((next_dir = readdir(dir)) != NULL)
    {
        if (strcmp(next_dir->d_name, ".") != 0 && strcmp(next_dir->d_name, "..") != 0)
        {
            char path[PATH_MAX];
            snprintf(path, sizeof(path), "%s/%s", dname,
                     next_dir->d_name);
            if (lstat(path, &sb) == -1)
            {
                perror("Error al llamar a lstat");
                return -1;
            }
            switch (sb.st_mode & __S_IFMT)
            {
            case __S_IFDIR:
                if (get_size_dir(path,
                                 blocks) == -1)
                {
                    perror("Error en get_size_dir");
                    return -1;
                }
                break;
            case __S_IFREG:
                *blocks += sb.st_blocks;
                break;
            default:
                break;
            }
        }
    }
    closedir(dir);
    return 0;
}
/* Processes all the files in the command line calling get_size on them to
* obtain the number of 512 B blocks they occupy and prints the total
size in
* kilobytes on the standard output
*/
int main(int argc, char *argv[])
{
    size_t blocks = 0;
    int i, exito;
    for (i = 1; i < argc; i++)
    {
        if (get_size(argv[i], &blocks) == 0)
        {
            printf("%ld K %s\n", blocks / 2, argv[i]);
        }
        else
        {
            perror("Error en get_size");
            exit(EXIT_FAILURE);
        }
    }
    return 0;
}