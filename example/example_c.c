#include <stdio.h>
#include <stdlib.h>

#include "demo_menu.h"
#include "stamen/stamen.h"

int operation1(size_t unused)
{
  (void)unused;

  printf("operation 1\n");
  printf("Some operation is done\n");
  return 1;
}

int operation2(size_t unused)
{
  (void)unused;

  printf("operation 2\n");
  printf("Some other operation is done\n");
  return 1;
}

int operation3(size_t unused)
{
  (void)unused;

  printf("operation 3\n");
  printf("Yet another operation is done\n");
  return 1;
}

int finish(size_t unused)
{
  (void)unused;

  printf("finishing...\n");
  exit(0);
}

int main(void)
{
  menu_main(0);
  return 0;
}
