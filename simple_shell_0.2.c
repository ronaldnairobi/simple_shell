#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_ARGS 64

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
char *args[MAX_ARGS];
int arg_count = 0;
char *token = strtok(input, " ");
while (token != NULL && arg_count < MAX_ARGS - 1)
{
args[arg_count++] = token;
token = strtok(NULL, " ");
}
args[arg_count] = NULL;
pid = fork();
if (pid == -1)
{
perror("fork");
exit(EXIT_FAILURE);
}
else if (pid == 0)
{
if (execvp(args[0], args) == -1)
{
perror("execvp");
exit(EXIT_FAILURE);}
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
