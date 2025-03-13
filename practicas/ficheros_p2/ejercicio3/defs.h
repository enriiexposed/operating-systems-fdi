#ifndef DEFS_H
#define DEFS_H
#define MAX_CHARS_NIF 9
#define MAX_PASSWD_LINE 255
typedef struct
{
	int student_id;
	char NIF[MAX_CHARS_NIF + 1];
	char *first_name;
	char *last_name;
} student_t;
/**
 * Index of the various fields in student_t
 * Helper data type to simplify the parser's implementation.
 */
typedef enum
{
	STUDENT_ID_IDX = 0,
	NIF_IDX,
	FIRST_NAME_IDX,
	LAST_NAME_IDX,
	NR_FIELDS_STUDENT
} token_id_t;
/**
 * Available actions supported by the program
 */
typedef enum
{
	NONE_ACT,
	PRINT_TEXT_ACT,
	PRINT_BINARY_ACT,
	WRITE_BINARY_ACT,
	ADD_ENTRIES_ACT
} action_t;
/**
 * Structure to hold the "variables" associated with
 * command-line options
 */
struct options
{
	char *input_file;
	char *output_file;
	action_t action;
};
#endif