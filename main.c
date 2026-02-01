#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// --- Shell Constants ---
#define LSH_RL_BUFSIZE 1024
#define LSH_TOK_BUFSIZE 64
#define LSH_TOK_DELIM " \t\r\n\a"

// ----------------------------------------------------------------------
// 1. EXECUTION LOGIC: LAUNCH & EXECUTE
// ----------------------------------------------------------------------

/**
 * @brief Launch a program and wait for it to terminate.
 * @param args Null-terminated list of arguments (including program name).
 * @return 1 (to continue the shell loop).
 */
int lsh_launch(char **args)
{
    pid_t pid, wpid;
    int status;

    // 1. Fork a child process
    pid = fork();

    if (pid == 0) {
        // Child process
        // Execute the program. 'execvp' searches the PATH for the command.
        if (execvp(args[0], args) == -1) {
            perror("lsh");
        }
        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        // Error forking
        perror("lsh");
    } else {
        // Parent process
        do {
            // Wait for the child process to change state (terminate)
            wpid = waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return 1;
}

/**
 * @brief Simple built-in 'exit' command handler.
 * @param args Not used.
 * @return 0 (to terminate the shell loop).
 */
int lsh_exit(char **args)
{
    return 0;
}

/**
 * @brief Simple built-in 'cd' (change directory) command handler.
 * @param args Array of arguments. Expected args[1] to be the directory.
 * @return 1 (to continue the shell loop).
 */
int lsh_cd(char **args)
{
    if (args[1] == NULL) {
        fprintf(stderr, "lsh: expected argument to \"cd\"\n");
    } else {
        // chdir is a system call that changes the current working directory.
        if (chdir(args[1]) != 0) {
            perror("lsh");
        }
    }
    return 1;
}

// List of built-in commands and their corresponding functions
char *builtin_str[] = {
    "cd",
    "exit"
};

int (*builtin_func[]) (char **) = {
    &lsh_cd,
    &lsh_exit
};

int lsh_num_builtins() {
    return sizeof(builtin_str) / sizeof(char *);
}

/**
 * @brief Check if command is built-in or launch as external process.
 * @param args Null-terminated list of arguments.
 * @return 0 if the shell should exit, 1 otherwise.
 */
int lsh_execute(char **args)
{
    if (args[0] == NULL) {
        // An empty command was entered.
        return 1;
    }

    // 1. Check for built-in commands
    for (int i = 0; i < lsh_num_builtins(); i++) {
        if (strcmp(args[0], builtin_str[i]) == 0) {
            return (*builtin_func[i])(args);
        }
    }

    // 2. Launch external command
    return lsh_launch(args);
}

// ----------------------------------------------------------------------
// 2. PARSING & READING LOGIC
// ----------------------------------------------------------------------

/**
 * @brief Read a line of input from stdin. Uses dynamic reallocation.
 * @return The line from stdin.
 */
char *lsh_read_line(void)
{
    char *line = NULL;
    size_t bufsize = 0; // getline allocates the buffer if set to 0 and line is NULL

    // getline is safer than fgets as it handles arbitrarily long lines
    if (getline(&line, &bufsize, stdin) == -1) {
        if (feof(stdin)) {
            exit(EXIT_SUCCESS);  // Exit on EOF (Ctrl+D)
        } else {
            perror("lsh: getline");
            exit(EXIT_FAILURE);
        }
    }
    return line;
}

/**
 * @brief Split a line into tokens (arguments).
 * @param line The line string to be parsed.
 * @return Null-terminated array of arguments.
 */
char **lsh_split_line(char *line)
{
    int bufsize = LSH_TOK_BUFSIZE, position = 0;
    char **tokens = malloc(bufsize * sizeof(char*));
    char *token;

    if (!tokens) {
        fprintf(stderr, "lsh: allocation error\n");
        exit(EXIT_FAILURE);
    }

    // strtok splits the string based on the delimiters defined in LSH_TOK_DELIM
    token = strtok(line, LSH_TOK_DELIM);
    while (token != NULL) {
        tokens[position] = token;
        position++;

        // If we exceed the buffer, reallocate more space
        if (position >= bufsize) {
            bufsize += LSH_TOK_BUFSIZE;
            tokens = realloc(tokens, bufsize * sizeof(char*));
            if (!tokens) {
                fprintf(stderr, "lsh: allocation error\n");
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, LSH_TOK_DELIM);
    }
    tokens[position] = NULL; // Null-terminate the list of arguments
    return tokens;
}

// ----------------------------------------------------------------------
// 3. SHELL LOOP & MAIN
// ----------------------------------------------------------------------

/**
 * @brief The main loop of the shell.
 */
void lsh_loop(void)
{
    char *line;
    char **args;
    int status;

    do {
        printf("> "); // Shell prompt
        
        line = lsh_read_line();       // Read command
        args = lsh_split_line(line);  // Parse command
        status = lsh_execute(args);   // Execute command

        // Cleanup: free the memory allocated by lsh_read_line and lsh_split_line
        free(line);
        free(args);
    } while (status);
}

/**
 * @brief Entry point of the program.
 */
int main(int argc, char **argv)
{
    // Run command loop.
    lsh_loop();

    // Perform any shutdown/cleanup.
    return EXIT_SUCCESS;
}