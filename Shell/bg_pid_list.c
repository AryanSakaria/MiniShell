#include "main.h"
void insert(int n, node **head, char *cname)
{
  char pid_str[7] = "";
  sprintf(pid_str, "%d", n);

  char path[30] = "";
  strcat(path, "/proc/");
  strcat(path, pid_str);
  strcat(path, "/exe");

  node *new = (node *)malloc(sizeof(node));

  new->v = n;
  memset(new->pname, 0, sizeof(new->pname));
  memset(new->cname, 0, sizeof(new->cname));
  strcpy(new->cname, cname);
  int len = readlink(path, new->pname, sizeof(new->pname));
  new->pname[len] = '\0';
  new->status = 1;
  new->next = NULL;

  if(*head == NULL)
  {
    *head = new;
    return;
  }

  new->next = *head;
  *head = new;
}

void show(node *head)
{
  while(head != NULL)
  {
    printf("%d\n", head->v);

    head = head->next;
  }
}

int delete(int n, node **head)
{
  if(*head == NULL)
  {
    return -1;
  }

  node *temp = *head;
  if((*head)->v == n)
  {
    *head = (*head)->next;
    free(temp);
    return 0;
  }

  node *prev = temp;

  while(temp->v != n && temp->next != NULL)
  {
    prev = temp;
    temp = temp->next;
  }

  if(temp->v == n)
  {
    prev->next = temp->next;
    free(temp);

    return 0;
  }

  return -1;
}