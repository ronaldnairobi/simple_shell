#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void)
{
char *input = NULL;
size_t len = 0;
ssize_t nread;
pid_t pid;

while (1)
{
printf("simple_shell$ ");
nread = getline(&input, &len, stdin);

if (nread == -1)
{
if (feof(stdin))
{
printf("\n");
break;
}
else
{
perror("getline");
exit(EXIT_FAILURE);
}
}
input[strcspn(input, "\n")] = '\0';
pid = fork();
if (pid == -1)
{
perror("fork");
exit(EXIT_FAILURE);
}
else if (pid == 0)
{
char **args = malloc(2 * sizeof(char*));
if (args == NULL)
{
perror("malloc");
exit(EXIT_FAILURE);
}
args[0] = input;
args[1] = NULL;
if (execve(input, args, NULL) == -1)
{
perror("execve");
exit(EXIT_FAILURE);
}
}
else
{
int status;
waitpid(pid, &status, 0);
}
}
free(input);
return 0;
}
