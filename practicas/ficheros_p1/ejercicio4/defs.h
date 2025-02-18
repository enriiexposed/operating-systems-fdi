#ifndef DEFS_H
#define DEFS_H

#define MAX_LOGIN_NAME  50
#define MAX_PASSWD_LINE  255

/**
 * Structure that describes an entry in /etc/passwd
 * Check "man 5 passwd" for details
 */
typedef struct passwd_entry {
	char login_name[MAX_LOGIN_NAME];
	char* optional_encrypted_passwd;
	int uid;
	int gid;
	char* user_name;
	char* user_home;
	char* user_shell;
} passwd_entry_t;

/**
 * Index of the various fields in passwd_entry_t
 * Helper data type to simplify the parser's implementation.
 */
typedef enum {
	LOGIN_NAME_IDX=0,
	ENCRYPTED_PASS_IDX,
	UID_IDX,
	GID_IDX,
	USER_NAME_IDX,
	USER_HOME_IDX,
	USER_SHELL_IDX,
	NR_FIELDS_PASSWD
} token_id_t;

/**
 * Output modes supported by the program
 */
typedef enum {
	VERBOSE_MODE,
	PIPE_MODE
} output_mode_t;

/**
 * Structure to hold the "variables" associated with
 * command-line options
 */
struct options {
	FILE* outfile;
	output_mode_t output_mode;
	char* input_mode;
};

#endif