#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <bits/getopt_core.h>

void free_argv(char **cmd_argv, int argc)
{
    int i;
    for (i = 0; i < argc; i++)
    {
        free(cmd_argv[i]);
    }
    free(cmd_argv);
}

pid_t launch_command(char **argv)
{
    pid_t pid = fork();
    if (pid < 0)
    {
        perror("No se ha creado el proceso hijo");
        return -1;
    }
    else if (pid == 0)
    {
        execvp(argv[0], argv);
    }
    return pid;
}

char **parse_command(const char *cmd, int *argc)
{
    // Allocate space for the argv array (initially with space for 10 args)
    size_t argv_size = 10;
    const char *end;
    size_t arg_len;
    int arg_count = 0;
    const char *start = cmd;
    char **argv = malloc(argv_size * sizeof(char *));

    if (argv == NULL)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    while (*start && isspace(*start))
        start++; // Skip leading spaces

    while (*start)
    {
        // Reallocate more space if needed
        if (arg_count >= argv_size - 1)
        { // Reserve space for the NULL at the end
            argv_size *= 2;
            argv = realloc(argv, argv_size * sizeof(char *));
            if (argv == NULL)
            {
                perror("realloc");
                exit(EXIT_FAILURE);
            }
        }

        // Find the start of the next argument
        end = start;
        while (*end && !isspace(*end))
            end++;

        // Allocate space and copy the argument
        arg_len = end - start;
        argv[arg_count] = malloc(arg_len + 1);

        if (argv[arg_count] == NULL)
        {
            perror("malloc");
            exit(EXIT_FAILURE);
        }
        strncpy(argv[arg_count], start, arg_len);
        argv[arg_count][arg_len] = '\0'; // Null-terminate the argument
        arg_count++;

        // Move to the next argument, skipping spaces
        start = end;
        while (*start && isspace(*start))
            start++;
    }

    argv[arg_count] = NULL; // Null-terminate the array

    (*argc) = arg_count; // Return argc

    return argv;
}

int main(int argc, char *argv[])
{
    char **cmd_argv;
    int cmd_argc;
    int i;

    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s \"command\"\n", argv[0]);
        return EXIT_FAILURE;
    }

    int opt = 0;
    pid_t pid;
    char *command;
    char *scriptfile;
    char background = 0;
    int status;

    while ((opt = getopt(argc, argv, "x:s:b")) != NULL)
    {
        switch (opt)
        {
        case 'x':
            command = optarg;
            break;
        case 's':
            scriptfile = optarg;
            break;
        case 'b':
            background = 1;
            break;
        }
    }

    if (command != NULL)
    {
        cmd_argv = parse_command(command, &cmd_argc);
        pid = launch_command(cmd_argv);
        if (waitpid(pid, &status, 0) == -1)
        {
            perror("Error waitpid");
            exit(EXIT_FAILURE);
        }
        else
            printf("Proceso %d terminado con estado %d", pid, status);
        freeargv(cmd_argv, cmd_argc);
    }
    else if (scriptfile != NULL)
    {
        int n = 0;
        char line[256];
        pid_t *b;
        setbuf(stdout, NULL);
        FILE *file = fopen(scriptfile, "r");
        if (!file)
        {
            perror("Error abriendo el archivo script");
            exit(EXIT_FAILURE);
        }
        while (fgets(line, 256, file) != NULL)
        {
            n++;
        }
        b = malloc(n * sizeof(pid_t));
        fseek(file, 0, SEEK_SET);
        for (i = 0; i < n; i++)
        {
            fgets(line, 256, file);
            cmd_argv = parse_command(line, &cmd_argc);
            printf("@@ Running command #%d: %s", i, line);
            pid = launch_command(cmd_argv);
            if (background == 0)
            {
                if (waitpid(pid, &status, 0) == -1)
                {
                    perror("Error waitpid");
                    exit(EXIT_FAILURE);
                }
                printf("@@ Command #%d terminated (pid: %d, status:%d)\n ", i, pid, status);
            }
            else
            {
                b[i] = pid;
            }
            free_argv(cmd_argv, cmd_argc);
        }
        if (background == 1)
        {
            for (i = 0; i < n; i++)
            {
                if ((pid = wait(&status)) == -1)
                {
                    perror("Error waitpid");
                    exit(EXIT_FAILURE);
                }
                int j = 0;
                while (j < n && b[j] != pid)
                {
                    j++;
                }
                if (j < n)
                {
                    printf("@@ Command #%d terminated (pid: %d, status: %d)\n ", j, pid, status);
                }
            }
        }
        free(b);
    }
    return EXIT_SUCCESS;
}