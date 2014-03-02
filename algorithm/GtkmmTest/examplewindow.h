#ifndef GTKMM_EXAMPLEWINDOW_H
#define GTKMM_EXAMPLEWINDOW_H

#include <gtkmm.h>

class ExampleWindow : public Gtk::Window
{
public:
  ExampleWindow();
  virtual ~ExampleWindow();

protected:
  //Signal handlers:
  void on_button_quit();
  void on_button_numbered(Glib::ustring data);

  //Child widgets:
  Gtk::Table m_Table;
  Gtk::Button m_Button_1, m_Button_2, m_Button_Quit;

};

#endif //GTKMM_EXAMPLEWINDOW_H