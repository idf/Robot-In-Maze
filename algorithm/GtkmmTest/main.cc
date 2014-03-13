#include "examplewindow.h"
#include <gtkmm/main.h>

int main (int argc, char *argv[])
{
  Gtk::Main app(argc, argv);

  IdleExample example;
  Gtk::Main::run(example);

  return 0;
}