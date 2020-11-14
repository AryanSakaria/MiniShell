#include "main.h"
int launch_loop(char **flags, int *BgCmd, node **head)
{
	pid_t pid, wpid;
	int status;

	pid = fork();
	if(pid == 0)
	{
		signal(SIGINT,SIG_DFL);
		if(execvp(flags[0], flags))
		{
			printf("Error: %s command unrecognised\n", flags[0]);
		}
		exit(EXIT_FAILURE);

	}

	else if(*BgCmd != 1)
	{
		
			do {
   			   wpid = waitpid(pid, &status, WUNTRACED);
   			 } while (!WIFEXITED(status) && !WIFSIGNALED(status));
	}

	else if(*BgCmd == 1)
	{
		printf("[+] %d process running \n", pid);

		usleep(100);
		insert(pid, head, flags[0]);
	}
	return 1;
}