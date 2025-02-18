#include <stdio.h>
#include <unistd.h> /* for getopt() */
#include <stdlib.h> /* for EXIT_SUCCESS, EXIT_FAILURE */
#include <string.h>
#include "defs.h"

char *loadstr(FILE *file);

/* Assume lines in the text file are no larger than 100 chars */
#define MAXLEN_LINE_FILE 100

/**
 * @brief Make a copy of existing string allocating memory accordingly
 *
 * @param original
 * @return new string that is a clone of original
 **/
static inline char* clone_string(char* original) {
    char* copy;
    copy = malloc(strlen(original) + 1);
    strcpy(copy, original);
    return copy;
}

/**
 * @brief Free up the array of entries, including the
 * memory of dynamically allocated strings
 *
 * @param entries
 * @param nr_entries
 */
void free_entries(student_t* entries, int nr_entries) {
    int i = 0;
    student_t* entry;
    for (i = 0; i < nr_entries; i++) {
        entry = &entries[i]; /* Point to current entry */
        free(entry->first_name);
        free(entry->last_name);
    }
    free(entries);
}

student_t* read_student_text_file(FILE* students, int* nr_entries) {
    char line[MAX_PASSWD_LINE + 1];
    student_t* entries;
    student_t* cur_entry;
    token_id_t token_id;
    char* token;
    char* lineptr;
    int entry_count;
    int i;

    fgets(line, MAX_PASSWD_LINE + 1, students);
    sscanf(line, "%d", &entry_count);
    entries = malloc(sizeof(student_t) * entry_count);
    /* zero fill the array of structures */
    memset(entries, 0, sizeof(student_t) * entry_count);

    for (i = 0; i < entry_count; i++) {
        fgets(line, MAX_PASSWD_LINE + 1, students);
        token_id = STUDENT_ID_IDX;
        lineptr = line;
        cur_entry = &entries[i];
        while ((token = strsep(&lineptr, ":")) != NULL) {
            switch (token_id) {
                case STUDENT_ID_IDX:
                    if (sscanf(token, "%d", &cur_entry->student_id) != 1) {
                        fprintf(stderr, "Warning: Wrong format for student_id field\n");
                        cur_entry->student_id = 0;
                    }
                    break;
                case NIF_IDX:
                    strcpy(cur_entry->NIF, token);
                    break;
                case FIRST_NAME_IDX:
                    cur_entry->first_name = clone_string(token);
                    break;
                case LAST_NAME_IDX:
                    cur_entry->last_name = clone_string(token);
                    break;
                default:
                    break;
            }
            token_id++;
        }
        if (token_id != NR_FIELDS_STUDENT) {
            fprintf(stderr, "Could not process all tokens\n");
            return NULL;
        }
    }
    (*nr_entries) = entry_count;
    return entries;
}

student_t* read_student_binary_file(FILE* students, int* nr_entries) {
    int entry_count;
    student_t* entries;
    int i;

    fread(&entry_count, 1, sizeof(int), students);
    entries = malloc(sizeof(student_t) * entry_count);
    /* zero fill the array of structures */
    memset(entries, 0, sizeof(student_t) * entry_count);

    for (i = 0; i < entry_count; i++) {
        fread(&entries[i].student_id, 1, sizeof(int), students);
        fread(entries[i].NIF, 1, sizeof(entries[i].NIF), students);
        entries[i].first_name = loadstr(students);
        entries[i].last_name = loadstr(students);
    }
    (*nr_entries) = entry_count;
    return entries;
}

int print_text_file(char *path) {
    /* To be completed (part A) */
    FILE* file;
    student_t* entries;
    int nr_entries;
    int i;

    if ((file = fopen(path, "r")) == NULL) {
        fprintf(stderr, "The input file %s could not be opened: ", path);
        perror(NULL);
        exit(EXIT_FAILURE);
    }
    entries = read_student_text_file(file, &nr_entries);
    for (i = 0; i < nr_entries; i++) {
        fprintf(stdout, "[Entry #%d]\n", i);
        student_t* cur_entry = &entries[i];
        fprintf(stdout, "student_id=%d\n", cur_entry->student_id);
        fprintf(stdout, "NIF=%s\n", cur_entry->NIF);
        fprintf(stdout, "first_name=%s\n", cur_entry->first_name);
        fprintf(stdout, "last _name=%s\n", cur_entry->last_name);
    }
    fclose(file);
    return 0;
}

int print_binary_file(char *path) {
    /* To be completed (part C) */
    FILE* file;
    student_t* entries;
    int nr_entries;
    int i;

    if ((file = fopen(path, "r")) == NULL) {
        fprintf(stderr, "The input file %s could not be opened: ", path);
        perror(NULL);
        exit(EXIT_FAILURE);
    }
    entries = read_student_binary_file(file, &nr_entries);
    for (i = 0; i < nr_entries; i++) {
        fprintf(stdout, "[Entry #%d]\n", i);
        student_t* cur_entry = &entries[i];
        fprintf(stdout, "student_id=%d\n", cur_entry->student_id);
        fprintf(stdout, "NIF=%s\n", cur_entry->NIF);
        fprintf(stdout, "first_name=%s\n", cur_entry->first_name);
        fprintf(stdout, "last_name=%s\n", cur_entry->last_name);
    }
    free_entries(entries, nr_entries);
    fclose(file);
    return 0;
}

char *loadstr(FILE *file) {
    int length = 0;
    int init = ftell(file);
    int c;

    while ((c = fgetc(file)) != EOF && c != '\0') {
        length++;
    }
    if (c == EOF) {
        return NULL;
    }
    fseek(file, init, SEEK_SET);
    char* str = (char*) malloc((length + 1) * sizeof(char));
    fread(str, sizeof(char), length + 1, file);
    return str;
}

int write_binary_file(char *input_file, char *output_file) {
    /* To be completed (part B) */
    FILE* file;
    FILE* bfile;
    student_t* entries;
    student_t* cur_entry;
    int nr_entries;
    int i;

    if ((file = fopen(input_file, "r")) == NULL) {
        fprintf(stderr, "The input file %s could not be opened: ", input_file);
        perror(NULL);
        exit(EXIT_FAILURE);
    }
    if ((bfile = fopen(output_file, "wb")) == NULL) {
        fprintf(stderr, "The input file %s could not be opened: ", output_file);
        perror(NULL);
        exit(EXIT_FAILURE);
    }
    entries = read_student_text_file(file, &nr_entries);
    fwrite(&nr_entries, 1, sizeof(int), bfile);
    for (i = 0; i < nr_entries; i++) {
        cur_entry = &entries[i];
        fwrite(&cur_entry->student_id, 1, sizeof(int), bfile);
        fwrite(cur_entry->NIF, 1, sizeof(cur_entry->NIF), bfile);
        fwrite(cur_entry->first_name, 1, strlen(cur_entry->first_name) + 1, bfile);
        fwrite(cur_entry->last_name, 1, strlen(cur_entry->last_name) + 1, bfile);
    }
    printf("%d student records written successfully to binary file %s\n", nr_entries, output_file);
    free_entries(entries, nr_entries);
    fclose(file);
    fclose(bfile);
    return 0;
}

int add_entries(char* input_file, int argc, char *argv[]) {
    FILE* file;
    char* extension;
    token_id_t token_id;
    char* lineptr;
    char* token;
    int i;
    int nr_entries;

    extension = strrchr(input_file, '.');
    if (strcmp(extension, ".bin") == 0) {
        if ((file = fopen(input_file, "r+")) == NULL) {
            fprintf(stderr, "The input file %s could not be opened: ", input_file);
            perror(NULL);
            exit(EXIT_FAILURE);
        }
        fread(&nr_entries, 1, sizeof(int), file);
        nr_entries += argc - optind;
        fseek(file, 0, SEEK_SET);
        fwrite(&nr_entries, 1, sizeof(int), file);
        fclose(file);
        if ((file = fopen(input_file, "a+")) == NULL) {
            fprintf(stderr, "The input file %s could not be opened: ", input_file);
            perror(NULL);
            exit(EXIT_FAILURE);
        }
        for (i = optind; i < argc; i++) {
            lineptr = argv[i];
            token_id = STUDENT_ID_IDX;
            while ((token = strsep(&lineptr, ":")) != NULL) {
                switch (token_id) {
                    case STUDENT_ID_IDX:
                        fwrite(token, 1, sizeof(int), file);
                        break;
                    case NIF_IDX:
                        fwrite(token, 1, strlen(token), file);
                        break;
                    case FIRST_NAME_IDX:
                        fwrite(token, 1, strlen(token) + 1, file);
                        break;
                    case LAST_NAME_IDX:
                        fwrite(token, 1, strlen(token) + 1, file);
                        break;
                    default:
                        break;
                }
                token_id++;
            }
            if (token_id != NR_FIELDS_STUDENT) {
                fprintf(stderr, "Could not process all tokens\n");
                return -1;
            }
        }
    } else {
        if ((file = fopen(input_file, "r+")) == NULL) {
            fprintf(stderr, "The input file %s could not be opened: ", input_file);
            perror(NULL);
            exit(EXIT_FAILURE);
        }
        fscanf(file, "%d", &nr_entries);
        nr_entries += argc - optind;
        fseek(file, 0, SEEK_SET);
        fprintf(file, "%d\n", nr_entries);
        fclose(file);
        if ((file = fopen(input_file, "a+")) == NULL) {
            fprintf(stderr, "The input file %s could not be opened: ", input_file);
            perror(NULL);
            exit(EXIT_FAILURE);
        }
        for (i = optind; i < argc; i++) {
            token = argv[i];
            fprintf(file, "%s\n", token);
        }
    }
    fclose(file);
    return 0;
}

int main(int argc, char *argv[]) {
    int ret_code, opt;
    struct options options;
    /* Initialize default values for options */
    options.input_file = NULL;
    options.output_file = NULL;
    options.action = NONE_ACT;
    ret_code = 0;

    /* Parse command-line options (incomplete code!) */
    while ((opt = getopt(argc, argv, "hi:po:ba")) != -1) {
        switch (opt) {
            case 'h':
                fprintf(stderr, "Usage: %s [ -h | -p | -i file | -o <output_file> | -b | -a ]\n", argv[0]);
                exit(EXIT_SUCCESS);
            case 'i':
                options.input_file = optarg;
                break;
            case 'p':
                options.action = PRINT_TEXT_ACT;
                break;
            case 'o':
                options.output_file = optarg;
                options.action = WRITE_BINARY_ACT;
                break;
            case 'b':
                options.action = PRINT_BINARY_ACT;
                break;
            case 'a':
                options.action = ADD_ENTRIES_ACT;
                break;
            default:
                exit(EXIT_FAILURE);
        }
    }

    if (options.input_file == NULL) {
        fprintf(stderr, "Must specify one record file as an argument of -i\n");
        exit(EXIT_FAILURE);
    }

    switch (options.action) {
        case NONE_ACT:
            fprintf(stderr, "Must indicate one of the following options: -p, -o, -b \n");
            ret_code = EXIT_FAILURE;
            break;
        case PRINT_TEXT_ACT:
            /* Part A */
            ret_code = print_text_file(options.input_file);
            break;
        case WRITE_BINARY_ACT:
            /* Part B */
            ret_code = write_binary_file(options.input_file, options.output_file);
            break;
        case PRINT_BINARY_ACT:
            /* Part C */
            ret_code = print_binary_file(options.input_file);
            break;
        case ADD_ENTRIES_ACT:
            /* Part 2 opcional */
            ret_code = add_entries(options.input_file, argc, argv);
            break;
        default:
            break;
    }
    exit(ret_code);
}