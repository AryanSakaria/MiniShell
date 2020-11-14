#include "main.h"
void she_loop()
{
	char *query;
	char **cmd;
	char **flags;
	struct utsname user;
	uname(&user);
	char cwd[PATH_MAX];
	getcwd(cwd, sizeof(cwd));
	char home[PATH_MAX];
	strcpy(home, cwd);
	int pos = strlen(cwd);
	char *temp;

	node *head = NULL;

	
	signal(SIGTSTP, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	while(1)
	{
		signal(SIGINT, SIG_IGN);


		getcwd(cwd, sizeof(cwd));
		
		temp = strlen(cwd) < pos ? cwd : (cwd + pos);
		
		printf("\x1b[31m" "<%s@%s:~%s>" "\x1b[0m", user.nodename, user.sysname, temp);
		query  = input();
		cmd = breakCmds(query);

		int i = 0;
		int BgCmd = 0;

		while(cmd[i] != NULL)
		{
			
			if(check_pipe(cmd[i]) == 0)
			{
				flags = parse(cmd[i], &BgCmd);
				shell_exec(flags, home, &BgCmd, &head);
			}

			else
			{
				//flags is parsed by pipes
				flags = pipeParse(cmd[i]);
				pipe_exec(flags, home);
			}

			i++;
			free(flags);
		}
		free(cmd);
		check_bg_proc(&head);
					
	}

}
