#include "main.h"

char **pipeParse(char *str)
{
	int itr = 0; int i = 0;
	int sizeInc = 10;

	char *query;
	query = strtok(str,"|\n");

	char **comds = (char **)malloc(10*sizeof(char *));

	while(query != NULL)
	{
		comds[itr] = (char *)malloc(strlen(query)+1);
		strcpy(comds[itr], query);
		itr++;
		if (itr >= sizeInc)
		{
			sizeInc += 10;
			comds = realloc(comds, sizeInc * sizeof(char *));
		}
		query = strtok(NULL, "|\n");
	}

	comds[itr] = NULL;
	return comds;
}


char **breakCmds(char *str)
{
	int itr = 0; int i = 0;
	int sizeInc = 10;

	char *query;
	query = strtok(str,";\n");

	char **comds = (char **)malloc(10*sizeof(char *));

	while(query != NULL)
	{
		comds[itr] = (char *)malloc(strlen(query)+1);
		strcpy(comds[itr], query);
		itr++;
		if (itr >= sizeInc)
		{
			sizeInc += 10;
			comds = realloc(comds, sizeInc * sizeof(char *));
		}
		query = strtok(NULL, ";\n");
	}

	comds[itr] = NULL;
	return comds;
}

char **parse(char *str, int *BgCmd)
{
   int BufSizeIncrement = 64;
   int pos = 0;
   char ** args = malloc(BufSizeIncrement * sizeof(char *));
   const char s[10] = " \t\r\n\a";
   char *token;


   /* get the first token */
   token = strtok(str, s);
	       
   /* walk through other tokens */
   while( token != NULL )
   {

   	   if(strcmp(token,"&"))
   	   {
   	   	args[pos] = (char *) malloc(strlen(token)+1);
   	   	strcpy(args[pos], token);
   	   	pos++;
   	   }

   	   else
   	   {
   	   	*BgCmd = 1;
   	   }
	   
	   if( pos >= BufSizeIncrement )
	   {
		   BufSizeIncrement += 64;
		   args = realloc(args, BufSizeIncrement * sizeof(char*));
	   }
	   token = strtok(NULL, s);
   }

   args[pos] = NULL;
      
   return args;

}