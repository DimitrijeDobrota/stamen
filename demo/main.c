#include "demo_menu.h"
#include "stamenc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const display_f display = stamen_builtin_display;

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

int menu_static_run(void) { return menu_main(); }
int menu_dynamic_run(void) { return stamen_start("menu_main"); }

int menu_dynamic_print(void) {
  stamen_print("menu_main");
  return 1;
}

int main(int argc, const char *argv[]) {
  const char *filename = "./demo_menu.conf";
  char buffer[FILENAME_MAX] = {0};
  const char *sep = strrchr(argv[0], '/');
  if (sep) {
    const size_t size = sep - argv[0] + 1;
    memcpy(buffer, argv[0], size);
    memcpy(buffer + size, filename, strlen(filename));
  }

  stamen_read(sep ? buffer : filename);
  stamen_insert("finish", finish);
  stamen_insert("operation1", operation1);
  stamen_insert("operation2", operation2);
  stamen_insert("operation3", operation3);

  static const item_t items[] = {
      {   menu_static_run, "Run statically generated menu"},
      {  menu_dynamic_run,              "Run dynamic menu"},
      {menu_dynamic_print,            "Print dynamic menu"},
      {            finish,                          "Quit"},
  };
  display("Menu demo program", items, sizeof(items) / sizeof(item_t));

  return 0;
}
