#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

/* Declare the global environment variable */
extern char **environ;

/**
* trim_whitespace - Removes leading and trailing whitespace from a string.
* @str: The string to trim.
*/
void trim_whitespace(char *str)
{
char *start = str;
char *end;

/* Find the first non-space character */
while (*start == ' ')
start++;

/* Find the last non-space character */
end = start + strlen(start) - 1;
while (end > start && *end == ' ')
end--;

/* Write null terminator */
*(end + 1) = '\0';

/* Move trimmed string to the original buffer */
memmove(str, start, strlen(start) + 1);
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
char **args = malloc(10 * sizeof(char *)); /* Adjust size if needed */
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
int is_interactive = isatty(STDIN_FILENO); /* Detect interactive mode */

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

trim_whitespace(buffer); /* Trim leading and trailing spaces */

if (strlen(buffer) == 0) /* Ignore empty or blank input */
continue;

if (strcmp(buffer, "exit") == 0) /* Exit the shell if "exit" is typed */
break;

pid = fork(); /* Fork to create a child process */
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
wait(&status); /* Wait for the child process to finish */
}
}

free(buffer); /* Free allocated buffer */
return (0);
}
