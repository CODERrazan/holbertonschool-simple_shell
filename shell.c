#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

/* Declare the global environment variable */
extern char **environ;

/**
 * tokenize_command - Splits a string into an array of arguments.
 * @buffer: The string to tokenize.
 *
 * Return: A dynamically allocated array of strings.
 */
char **tokenize_command(char *buffer)
{
    char *token;
    char **args = malloc(10 * sizeof(char *)); /* Allocate memory */
    int i = 0;

    if (!args)
    {
        perror("Allocation error");
        exit(EXIT_FAILURE);
    }

    token = strtok(buffer, " ");
    while (token != NULL)
    {
        args[i++] = token;
        token = strtok(NULL, " ");
    }
    args[i] = NULL;

    return args;
}

/**
 * main - Entry point for the shell program.
 *
 * Return: Always 0.
 */
int main(void)
{
    char *buffer = NULL;
    size_t bufsize = 0;
    ssize_t characters;
    pid_t pid;
    int status;
    int is_interactive = isatty(STDIN_FILENO); /* Check interactive mode */

    while (1) /* Loop to continuously run the shell */
    {
        if (is_interactive) /* Display prompt only in interactive mode */
            write(STDOUT_FILENO, "#cisfun$ ", 9);

        characters = getline(&buffer, &bufsize, stdin); /* Read user input */

        if (characters == -1) /* Handle EOF (Ctrl+D) */
        {
            if (is_interactive)
                write(STDOUT_FILENO, "\n", 1);
            break;
        }

        buffer[characters - 1] = '\0'; /* Remove newline character */

        if (strlen(buffer) == 0) /* Ignore empty input */
            continue;

        if (strcmp(buffer, "exit") == 0) /* Exit the shell */
            break;

        pid = fork(); /* Create a child process */
        if (pid == -1)
        {
            perror("Error:");
            break;
        }
        if (pid == 0) /* Child process */
        {
            char **args = tokenize_command(buffer); /* Tokenize input */

            if (execve(args[0], args, environ) == -1) /* Execute command */
            {
                perror("./shell");
                free(args);
                exit(EXIT_FAILURE);
            }
        }
        else /* Parent process */
        {
            wait(&status); /* Wait for the child process */
        }
    }

    free(buffer); /* Free allocated buffer */
    return (0);
}
