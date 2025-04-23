#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/stat.h>

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
char **args;
int i = 0;

args = malloc(10 * sizeof(char *));
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
char **args;
size_t bufsize = 0;
ssize_t characters;
pid_t pid;
int status;
int is_interactive = isatty(STDIN_FILENO);

while (1)
{
if (is_interactive)
write(STDOUT_FILENO, ":) ", 3);

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
{
free(buffer);
exit(0);
}

args = tokenize_command(buffer);

/* Directly execute the provided command */
if (access(args[0], X_OK) != 0)
{
fprintf(stderr, "%s: command not found\n", args[0]);
free(args);
continue;
}

pid = fork();
if (pid == -1)
{
perror("Error:");
free(args);
break;
}
if (pid == 0)
{
if (execve(args[0], args, environ) == -1)
{
perror("./shell");
free(args);
exit(2);
}
}
else
{
wait(&status);
if (WIFEXITED(status))
status = WEXITSTATUS(status);
}

free(args);
}

free(buffer);
return (status);
}
