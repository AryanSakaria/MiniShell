#include<unistd.h>
#include<fcntl.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/utsname.h>
#include<limits.h>
#include<stdlib.h>
#include<string.h>
#include<sys/wait.h>
#include<dirent.h>
#include<pwd.h>
#include<grp.h>
#include<sys/stat.h>
#include<time.h>

typedef struct node
{
 
  int v;
  int status;
  char pname[50];
  char cname[50];
  struct node *next;
} node;



char *input();
char **breakCmds(char *str);
char **parse(char *str, int *BgCmd);
void ls_exec(char **flags);
int pwd_exec();
int cd_exec(char **flags, char *home);
int echo_exec(char **flags);
int check_path(char *arg);
void call_pinfo(char *pid_str);
void shell_exec(char **flags, char *home, int *BgCmd,  node **head);
void she_loop();
void insert(int n, node **head, char *cname);
void show(node *head);
int delete(int n, node **head);
void check_bg_proc(node **head);
int launch_loop(char **flags, int *BgCmd, node **head);
int check_red(char **flags);
int check_pipe(char *flags);
char **pipeParse(char *str);
void pipe_exec(char **flags, char *home);
