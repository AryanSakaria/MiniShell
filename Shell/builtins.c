#include "main.h"
int pwd_exec()
{
	char cwd[PATH_MAX];
	getcwd(cwd, sizeof(cwd));
	printf("%s\n", cwd);
}


int cd_exec(char **flags, char *home)
{
	if(flags[1] == NULL)
	{
		if(chdir(home))
		{
			perror("ERROR");
		}
	}
	else if (!strcmp(flags[1],"~"))
	{
		//printf("Detecting Tilda\n");
		//printf("%s\n", home );
		if(chdir(home))
		{
			perror("ERROR");
		}
	}	

	else if ( strstr(flags[1], "~") != NULL )
	{
		//printf("Coming here\n");
		char temp[PATH_MAX];
		strcpy(temp, home);
		strcat(temp, flags[1]+1);

		if(chdir(temp))
			perror("ERROR");

	}
	else if(chdir(flags[1]))
	{
		//printf("And here\n");
		perror("ERROR");
	}
}

int echo_exec(char **flags)
{
	char temp[150];
	int i = 1;
	while(flags[i]!=NULL)
	{
		printf("%s ", flags[i]);
		i++;
	}
	printf("\n");
}

int check_path(char *arg)
{
	if (strstr(arg,"-a")!= NULL)
	{
		return 0;
	}
	if (strstr(arg, "-l")!= NULL)
	{
		return 0;
	}

	if (strstr(arg, "-al")!= NULL)
	{
		return 0;
	}

	if(strstr(arg, "-la")!=NULL)
	{
		return 0;
	}

	return 1;
}

char temp_pinfo(char *pid_str)
{
	int pid;
	char temp[100];
 
  	pid = atoi(pid_str);
	

  char path[30] = "";

  char file[10000] = "";
  sprintf(temp, "%d", pid);

  

  memset(path, 0, sizeof(path));
  strcat(path, "/proc/");
  strcat(path, temp);
  strcat(path, "/stat");

  int fp = open(path, O_RDONLY);

  memset(file, 0, sizeof(file));
  read(fp, file, sizeof(file));

  // memset(line, 0, sizeof(line));
  char *line = strtok(file, " ");

  int line_num = 0;
  while(line != NULL && line_num <=23)
  {
    if(line_num == 2 )
    {
      
      return line[0];
    }
   
    line = strtok(NULL, " ");
    line_num++;
  }

}

void call_pinfo(char *pid_str)
{
	int pid;
	char temp[100];
  if(pid_str == NULL)
  	{

  		pid = getpid();


  	}
  else
  	{
  		pid = atoi(pid_str);
	}

  char path[30] = "";

  char file[10000] = "";
  sprintf(temp, "%d", pid);

  printf("Process Id %d\n", pid);

  memset(path, 0, sizeof(path));
  strcat(path, "/proc/");
  strcat(path, temp);
  strcat(path, "/stat");

  int fp = open(path, O_RDONLY);

  memset(file, 0, sizeof(file));
  read(fp, file, sizeof(file));

  // memset(line, 0, sizeof(line));
  char *line = strtok(file, " ");

  int line_num = 0;
  while(line != NULL && line_num <=23)
  {
    if(line_num == 2 )
    {
      fprintf(stdout, "Process status: %s\n", line);
    }
    else if(line_num == 23)
    {
    	fprintf(stdout, "Virtual Memory : %s\n", line);
    }
    line = strtok(NULL, " ");
    line_num++;
  }

  close(fp);

  memset(path, 0, sizeof(path));
  strcat(path, "/proc/");
  strcat(path, temp);
  strcat(path, "/exe");

  char exec_path[50] = "";
  int len = readlink(path, exec_path, sizeof(exec_path));
  exec_path[len] = '\0';

  fprintf(stdout, "Executable path: %s\n", exec_path);
}

void executeJobs(char **flags, node **head)
{
	node *temp = *head;
	int proc_count=0;
	while(temp!=NULL)
	{
		proc_count++;
		temp = temp->next;
	}
	printf("Proc count: %d\n", proc_count);
	temp = *head;
	int i = 1;
	while(temp!=NULL)
	{
		int id = temp->v;
		char pid_str[25];
		sprintf(pid_str, "%d", id);	
		if(temp_pinfo(pid_str)=='T')
		{
			printf("%d  [%d] %s Stopped  \n", i, temp->v, temp->cname);
		}
		else
		{
			printf("%d  [%d] %s Running  \n", i, temp->v, temp->cname);
		}
		temp = temp->next;
		i++;
	}
}

void over_kill(char **flags, node **head)
{
	node *temp = *head;
	while(temp!=NULL)
	{
		kill(temp->v, 9);
		int a = temp->v;
		temp = temp->next;
		delete(a, head);

	}
}


void call_setnv(char **flags)
{
	int size = 0;
	while(flags[size++]!=NULL);

	if(size == 2)
	{
		printf("Too few args.\n");
		return;
	}
	if(size > 4)
	{
		printf("Too many args\n" );
		return;
	}
	if(size == 3)
	{
		setenv(flags[1], " ", 1);

	}

	if(size == 4)
	{
		setenv(flags[1],flags[2], 1);
	}

	return;


}

void quit()
{
	exit(0);
}

void call_unsetenv(char **flags){
	int i = 0 ;
	int size = 0;
	while(flags[size++]!=NULL);

	if(size<2){
		printf("Error too few args\n");
		return ; 
	}

	for(i = 1 ; flags[i]!=NULL;++i){
		unsetenv(flags[i]);
	}

	return ; 


}





void k_job(char **flags, node **head)
{
	int size = 0;
	while(flags[size++]!=NULL);
	if(size < 3)
	{
		printf("Not enough Arguments\n");
		return;
	}
	int i = atoi(flags[1]);
	int sgnl = atoi(flags[2]);
	int stat;

	node *temp = *head;
	int counter = 1;

	while(temp!=NULL)
	{
		if((i==counter)&&(kill(temp->v,0)==0))
		{
			kill(temp->v, sgnl);
			while(waitpid(-1, &stat,WNOHANG)>0);

		}

		counter++;
		temp = temp->next;

	}

	if(counter < i)
	{
		printf("No job exists\n");
		return;
	}

}

void fg(char **flags, node **head)
{
	if(flags[1]==NULL)
	{
		printf("Job number not specified\n" );
		return;
	}

	else
	{
		node *temp = *head;
		int jno = atoi(flags[1]);

		int i = 0;
		for(i = 1; i < jno; ++i)
		{
			
			if(temp==NULL)
			{
				printf("Job number exceeds list\n");
				return;
			}
			temp = temp->next;
		}
		kill(temp->v, SIGCONT);
		printf("%d\n", temp->v);
		int stat;
		waitpid(temp->v, &stat, WUNTRACED);

	}

}

void bg(char **flags, node **head)
{
	if(flags[1]==NULL)
	{
		printf("Job number not specified\n" );
		return;
	}
	else
	{
		node *temp = *head;
		int jno = atoi(flags[1]);

		int i = 0;
		for(i = 1; i < jno; ++i)
		{
			
			if(temp==NULL)
			{
				printf("Job number exceeds list\n");
				return;
			}

			temp = temp->next;
		}
		kill(temp->v, SIGCONT);
		//printf("%d\n", temp->v);
		//int stat;
		//waitpid(temp->v, &stat, WUNTRACED);

	}

}

void shell_exec(char **flags, char *home, int *BgCmd,  node **head)
{
	//if I need to redirect
	//else exec this stuff:
	

	if(check_red(flags)==0)
	{
		
		if (strcmp(flags[0],"pwd")==0)
		{
			pwd_exec();
		}

		else if (strcmp(flags[0], "cd")==0)
		{
			cd_exec(flags, home);
		}

		else if(strcmp(flags[0], "echo")==0)
		{
			echo_exec(flags);
		}

		else if(strcmp(flags[0], "ls")==0)
		{
			ls_exec(flags);
		}
		else if (strcmp(flags[0],"setenv")==0)
		{
			call_setnv(flags);	
		}

		else if(strcmp(flags[0],"unsetenv")==0)
		{

			call_unsetenv(flags);
		}

		else if(strcmp(flags[0],"overkill")==0)
		{
			over_kill(flags, head);
			
		}
		else if(strcmp(flags[0],"fg")==0)
		{
			fg(flags, head);
		}
		else if(strcmp(flags[0],"bg")==0)
		{
			bg(flags, head);
		}

		else if(strcmp(flags[0], "pinfo")==0)
		{
			call_pinfo(flags[1]);
		}
		else if(strcmp(flags[0],"kjobs")==0)
		{
			k_job(flags, head);
		}
		else if (strcmp(flags[0], "jobs")==0)
		{
			executeJobs(flags, head);
		}
		else if(strcmp(flags[0],"quit")==0)
		{
			
			exit(0);
		}

		else launch_loop(flags, BgCmd, head);
	}

	else
	{
		
		int ic = 0;
		int fd;

		int initin = dup(0);
		int initout = dup(1);

		while(flags[ic] != NULL)
		{
			if (strcmp(flags[ic],"<") == 0) 
			{
				fd = open(flags[ic+1], O_RDONLY, 0);
				
				if (fd == -1)
				{
					perror("Input file failed to open");
				}

				else if ( dup2(fd, STDIN_FILENO) < 0)
				{
					perror("Error duplicating file descripter");
				}

				close(fd);
			}

			if (strcmp(flags[ic],">") == 0) 
			{
				fd = open(flags[ic+1], O_WRONLY | O_TRUNC | O_CREAT, 0644);
				
				if (fd == -1)
				{
					perror("Output file failed to open");
				}

				else if ( dup2(fd, STDOUT_FILENO) < 0)
				{
					perror("Error duplicating file descripter");
				}

				close(fd);
			}

			if (strcmp(flags[ic],">>") == 0) 
			{
				fd = open(flags[ic+1], O_WRONLY | O_APPEND | O_CREAT, 0644);
				
				if (fd == -1)
				{
					perror("Output append file failed to open");
				}

				else if ( dup2(fd, STDOUT_FILENO) < 0)
				{
					perror("Error duplicating file descripter");
				}

				close(fd);
			}

			ic++;
		}

		char **fags = (char **) malloc (sizeof(char *) * 20);
		int l = 0;
		ic = 0;
		while(flags[ic]!=NULL)
		{
			
			if ( (strcmp(flags[ic], ">>")==0) || (strcmp(flags[ic], ">")==0) || (strcmp(flags[ic], "<")==0))
			{
				break;
			} 

			fags[l] = (char *) malloc (sizeof(char) * 40);
			strcpy(fags[l], flags[ic]);
			ic++;
			l++;

		}

		fags[l] = NULL;






		if (strcmp(flags[0],"pwd")==0)
		{
			pwd_exec();
		}

		else if (strcmp(flags[0], "cd")==0)
		{
			cd_exec(fags, home);
		}

		else if(strcmp(flags[0], "echo")==0)
		{
			echo_exec(fags);
		}

		else if(strcmp(flags[0], "ls")==0)
		{
			ls_exec(fags);
		}
		
		else if(strcmp(flags[0], "pinfo")==0)
		{
			call_pinfo(fags[1]);
		}

		else launch_loop(fags, BgCmd, head);

		dup2(initin, 0);
		close(initin);
		dup2(initout, 1);
		close(initout);

	}

}







