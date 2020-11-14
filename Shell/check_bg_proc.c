#include "main.h"

void check_bg_proc(node **head)
{
  node *temp = *head;
  int status;
  int wait_ret;
  while(temp != NULL)
  {
      wait_ret = waitpid(temp->v, &status, WNOHANG);

      if(wait_ret != 0 && wait_ret !=  -1)
      {
        if(WIFEXITED(status))
        {
          fprintf(stderr, "\n[-] %d  exited successfully\n", temp->v);

          delete(temp->v, head);
        }
        else if(WIFSIGNALED(status))
        {
          fprintf(stderr, "\n[-] %d  interrupted\n", temp->v);

          delete(temp->v, head);
        }
      }

      temp = temp->next;
  }
}