#include "main.h"
void ls_exec(char **flags)
{
	unsigned int flag_a, flag_l, flag_al, flag_dirs;
	flag_a = flag_l = flag_al = flag_dirs = 0;
	char cwd[PATH_MAX];
	char path[PATH_MAX];
	getcwd(cwd, sizeof(cwd));

	int i = 1;
	//checking what type of flags are there
	while(flags[i] != NULL)
	{
		if (strstr(flags[i],"-a")!= NULL)
		{
			flag_a = 1;
		}

		if (strstr(flags[i], "-l")!= NULL)
		{
			flag_l = 1;
		}

		if (strstr(flags[i], "-al")!= NULL)
		{
			
			flag_a = 1;
			flag_l = 1;
		}

		if(strstr(flags[i], "-la")!=NULL)
		{
			
			flag_a = 1;
			flag_l = 1;
		}

		i++;
	}

	
	if (check_path(flags[i-1])==1)
	{
		//if a directory is specified
		if(i!=1)
		{
			flag_dirs = 1;
		}
	}

	if ( flag_dirs == 1)
	{
		strcpy(path, cwd);
		strcat(path, "/");
		strcat(path, flags[i-1]);
		
	}
	else
	{
		strcpy(path, cwd);
	}

	DIR *d;
	struct dirent *dir;
    

	d = opendir(path);
	//printf("%s\n", path );

	if(d)
	{

		if ((flag_l!=1))
		{
			while ((dir = readdir(d)) != NULL)
			 {

			 	char *fPath = (char *) malloc (140 * sizeof(char));
				strcpy(fPath, path);
				strcat(fPath,"/");
				strcat(fPath, dir->d_name);
				//printf("%s\n", fPath);

				struct stat fData ;
				struct passwd *oData = getpwuid(fData.st_uid);
				struct group *grData = getgrgid(fData.st_gid);
				// for some reason it's showing root on first ls
				stat(fPath, &fData);

			 	//implementing -a
			 	if(flag_a == 1)
			 	{
			 		S_ISDIR(fData.st_mode)  ? printf("\x1b[32m" "%s\n" "\x1b[0m", dir->d_name) : printf("%s\n", dir->d_name);
			 	}
			 	else
			 	{

			 		if(dir->d_name[0]!='.')
			 		{
			 			S_ISDIR(fData.st_mode)  ? printf("\x1b[32m" "%s\n" "\x1b[0m", dir->d_name) : printf("%s\n", dir->d_name);
			 		}
			 	}
				
	    	}
		}

		else
		{
			int i = 0;
			while ((dir = readdir(d)) != NULL)
			{
				if(flag_a != 1)
				{
					if(dir->d_name[0] == '.')
						continue;

				}	
				char *fPath = (char *) malloc (140 * sizeof(char));
				strcpy(fPath, path);
				strcat(fPath,"/");
				strcat(fPath, dir->d_name);
				//printf("%s\n", fPath);

				struct stat fData ;
				struct passwd *oData = getpwuid(fData.st_uid);
				struct group *grData = getgrgid(fData.st_gid);
				// for some reason it's showing root on first ls
				

				stat(fPath, &fData);


				char time[200];
				S_ISDIR(fData.st_mode)  ? printf("d") : printf("-");
				fData.st_mode & S_IRUSR ? printf("r") : printf("-");
				fData.st_mode & S_IWUSR ? printf("w") : printf("-");
				fData.st_mode & S_IXUSR ? printf("x") : printf("-");
				fData.st_mode & S_IRGRP ? printf("r") : printf("-");
				fData.st_mode & S_IWGRP ? printf("w") : printf("-");
				fData.st_mode & S_IXGRP ? printf("x") : printf("-");
				fData.st_mode & S_IROTH ? printf("r") : printf("-");
				fData.st_mode & S_IWOTH ? printf("w") : printf("-");
				fData.st_mode & S_IXOTH ? printf("x") : printf("-");
				printf(" ");

				printf(" %ld ", fData.st_nlink);

				printf("%-10s ", oData->pw_name);
				printf("%-10s ", grData->gr_name);	

				printf(" %10ld ", fData.st_size);

				
				strftime (time, sizeof(time), "%b %d %R", localtime(&(fData.st_ctim.tv_sec)));

				printf("%s ", time);
				S_ISDIR(fData.st_mode)  ? printf("\x1b[32m" "\t%s" "\x1b[0m", dir->d_name) : printf("\t%s", dir->d_name);
				
				printf("\n");
				
				
	    	}
			
		}
    
	}

	else
	{
		perror("Could not open directory. (No such file or directory) \n");
	}


   	closedir(d);
}