#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

/* Declare the global environment variable */
extern char **environ;

/**
 * find_command_path - Searches for a command in the PATH variable.
 * @command: The command to search for.
 *
 * Return: The full path if found, otherwise NULL.
 */
char *find_command_path(char *command)
{
    char *path, *token, *full_path;
    int path_length;

    path = getenv("PATH");
    if (!path)
        return NULL;

    token = strtok(path, ":");
    while (token)
    {
        path_length = strlen(token) + strlen(command) + 2;
        full_path = malloc(path_length);

        if (!full_path)
        {
            perror("Allocation error");
            exit(EXIT_FAILURE);
        }

        sprintf(full_path, "%s/%s", token, command);
        if (access(full_path, X_OK) == 0)
            return full_path;

        free(full_path);
        token = strtok(NULL, ":");
    }

    return NULL;
}

/**
 * tokenize_command - Splits a string into an array of arguments.
 * @buffer: The string to tokenize.
 *
 * Return: A dynamically allocated array of strings.
 */
char **tokenize_command(char *buffer)
{
    char *token;
    char **args = malloc(10 * sizeof(char *));
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
    char *buffer = NULL, *command_path;
    size_t bufsize = 0;
    ssize_t characters;
    pid_t pid;
    int status;
    int is_interactive = isatty(STDIN_FILENO); /* Detect interactive mode */
    char **args; /* Declare variables at start of block */

    while (1)
    {
        if (is_interactive)
            write(STDOUT_FILENO, ":) ", 3); /* Updated prompt */

        characters = getline(&buffer, &bufsize, stdin);
        if (characters == -1)
        {
            if (is_interactive)
                write(STDOUT_FILENO, "\n", 1);
            break;
        }

        buffer[characters - 1] = '\0';

        if (strlen(buffer) == 0)
            continue;

        if (strcmp(buffer, "exit") == 0)
            break;

        args = tokenize_command(buffer);

        /* If command is an absolute or relative path, use it directly */
        if (access(args[0], X_OK) == 0)
            command_path = args[0];
        else
            command_path = find_command_path(args[0]);

        if (!command_path) /* Prevent fork() if command doesn't exist */
        {
            fprintf(stderr, "%s: command not found\n", args[0]);
            free(args);
            continue;
        }

        pid = fork();
        if (pid == -1)
        {
            perror("Error:");
            break;
        }
        if (pid == 0) /* Child process */
        {
            if (execve(command_path, args, environ) == -1)
            {
                perror("./shell");
                free(args);
                exit(EXIT_FAILURE);
            }
        }
        else /* Parent process */
        {
            wait(&status);
        }

        if (command_path != args[0]) /* Free memory only if allocated */
            free(command_path);
        free(args);
    }

    free(buffer);
    return (0);
}
