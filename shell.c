#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

/* Declare the global environment variable */
extern char **environ;

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

if (strcmp(buffer, "exit") == 0) /* Exit the shell if "exit" is typed */
break;

pid = fork(); /* Fork to create a child process */
if (pid == -1) /* Handle fork failure */
{
perror("Error:");
break;
}
if (pid == 0) /* Child process */
{
char *args[2];
args[0] = buffer; /* Command name */
args[1] = NULL;   /* End of arguments */

if (execve(buffer, args, environ) == -1) /* Execute command */
{
perror("./shell");
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
