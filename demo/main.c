#include "demo_menu.h"
#include "stamenc.h"

#include <stdio.h>
#include <stdlib.h>

const stamen_display_f stamen_display = stamen_builtin_display;

int operation1(void) {
  printf("operation 1\n");
  printf("Some operation is done\n");
  return 1;
}

int operation2(void) {
  printf("operation 2\n");
  printf("Some other operation is done\n");
  return 1;
}

int operation3(void) {
  printf("operation 3\n");
  printf("Yet another operation is done\n");
  return 1;
}

int finish(void) {
  printf("finishing...\n");
  exit(0);
}

int menu_dynamic_print(void) {
  stamen_print("menu_main");
  return 1;
}

int main(void) {
  menu_main();
  return 0;
}
