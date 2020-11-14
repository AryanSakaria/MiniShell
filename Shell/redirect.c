#include "main.h"

int check_red(char **flags)
{
	int i = 0;

	while(flags[i]!=NULL)
	{
		if ( (strcmp(flags[i], ">>")==0) || (strcmp(flags[i], ">")==0) || (strcmp(flags[i], "<")==0))
		{
			return 1;
		} 

		i++;
	}

	return 0;

}

int check_pipe(char *flags)
{
	int i = 0;
	int flag = 0;
	while(flags[i]!='\0')
	{
		if ( flags[i] == '|' )
		{
			flag = 1;
		} 

		i++;
	}

	return flag;
}



void pipe_exec(char **flags, char *home)
{
	//if I need to redirect
	//else exec this stuff:

	int ic;
	int pipefd[2];
	int foolbg = 0;
	int initin = 0;
	int size = 0;
	int i;
	int inputflag = 0;
	int outputflag = 0;
	int fd = 0;
	char **commds;
	int initout = dup(STDOUT_FILENO);
	int in = dup(0);
	int out = dup(1);


	char cpy1st[1000];
	strcpy(cpy1st, flags[0]);

	int counter=0;
	for(int k=0;flags[k]!=NULL;++k) counter++;

	char cpylast[1000];
	strcpy(cpylast, flags[counter-1]);

	char **args1 = parse(cpy1st, &foolbg);
	char **args2 = parse(cpylast, &foolbg);

	int ein=0,eout=0;

	char **newargs1 = (char **)malloc(sizeof(char *) * 500);
	int nargs1=0,nargs2=0;
	char **newargs2 = (char **)malloc(sizeof(char *) * 500);

	for(i=0;i<500;++i)
	{
		newargs1[i] = NULL;
	}
	for(i=0;i<500;++i)
	{
		newargs2[i] = NULL;
	}


	


	while(flags[size]!=NULL)
	{
		size++;
		
	}

	i = 0;
 
	
	for(i=0; i < size; i++)
	{
		
		if ((i == 0) )
		{
			for(ic=0 ; args1[ic]!=NULL ; ++ic)
			{
				if(strcmp(args1[ic], "<")==0)
				{
					ein=1;
					int in;
					in = open(args1[ic+1], O_RDONLY);
					if(in == -1)
					{
						printf("error1");
						return ;
					}
					dup2(in, 0);
					close(in);
				}
				else if(!ein)
				{
					newargs1[nargs1] = (char *) malloc (sizeof(char ) * 40);
					strcpy(newargs1[nargs1],args1[ic]);
					nargs1++;
				}
			}

			commds = newargs1;
			
		}

		else if (i== (size-1))
		{


			for(ic=0 ; args2[ic]!=NULL ; ++ic)
			{
				if(strcmp(args2[ic], ">")==0)
				{
					eout=1;
					int out;
					out = open(args2[ic+1], O_WRONLY | O_TRUNC | O_CREAT, 0644);
					if(out == -1)
					{
						printf("error2");
						return ;
					}
					dup2(out, 1);
					close(out);
				}

				if(strcmp(args2[ic], ">>")==0)
				{
					eout=1;
					int out;
					out = open(args2[ic+1], O_WRONLY | O_APPEND | O_CREAT, 0644);
					if(out == -1)
					{
						printf("error2");
						return ;
					}
					dup2(out, 1);
					close(out);
				}
				else if(!eout)
				{
					newargs2[nargs2] = (char *) malloc (sizeof(char) * 40);
					strcpy(newargs2[nargs2],args2[ic]);
					nargs2++;
				}
			}

			commds = newargs2;
		}
		else
		{
			commds = parse(flags[i], &foolbg);
		}



		pipe(pipefd);

		pid_t pid = fork();

		if(pid == 0)
		{
			
			dup2(initin, 0);
			
			
			if(i != (size - 1))
			{
				dup2(pipefd[1], 1);
			}

			close(pipefd[0]);
			
			if (strcmp(commds[0],"pwd")==0)
			{
				pwd_exec();
				exit(1);
			}

			else if (strcmp(commds[0], "cd")==0)
			{
				cd_exec(commds, home);
				exit(1);
			}

			else if(strcmp(commds[0], "echo")==0)
			{
				echo_exec(commds);
				exit(1);
			}

			else if(strcmp(commds[0], "ls")==0)
			{
				ls_exec(commds);
				exit(1);
			}
			
			else if(strcmp(commds[0], "pinfo")==0)
			{
				call_pinfo(commds[1]);
				exit(1);
			}

			else if(execvp(commds[0], commds))
			{
				
				exit(1);
			}

		}


		else if(pid > 0)
		{

			wait(NULL);
			
			close(pipefd[1]);
			initin = pipefd[0];
			//exit(1);
			if(i==0)
			{
				dup2(in, 0);
				close(in);
				
			}

			if(i == (size-1))
			{
				dup2(out, 1);
				close(out);

			}

		}

	}

	dup2(STDIN_FILENO, initin);
	close(initin);
	dup2(STDOUT_FILENO, initout);
	close(initout);

	//
}