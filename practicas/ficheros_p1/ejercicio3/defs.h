/**
 * Output modes supported by the program
 */
typedef enum {
	ODD,
	EVEN,
} parity_t;

/**
 * Structure to hold the "variables" associated with
 * command-line options
 */
struct options {
	char* title;
	parity_t par_mode;
	int lenght;
};
