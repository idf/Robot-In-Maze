#include "examplewindow.h"

IdleExample::IdleExample() :
  m_Box(false, 5),
  m_ButtonQuit(Gtk::Stock::QUIT)
{
  set_border_width(5);

  // Put buttons into container

  // Adding a few widgets:
  add(m_Box);
  m_Box.pack_start( *Gtk::manage(new Gtk::Label("Formatting Windows drive C:")));
  m_Box.pack_start( *Gtk::manage(new Gtk::Label("100 MB")) );
  m_Box.pack_start(m_ProgressBar_c);

  m_Box.pack_start( *Gtk::manage(new Gtk::Label("")) );

  m_Box.pack_start( *Gtk::manage(new Gtk::Label("Formatting Windows drive D:")));
  m_Box.pack_start( *Gtk::manage(new Gtk::Label("5000 MB")) );
  m_Box.pack_start(m_ProgressBar_d);

  Gtk::HBox* hbox = Gtk::manage( new Gtk::HBox(false,10));
  m_Box.pack_start(*hbox);
  hbox->pack_start(m_ButtonQuit, Gtk::PACK_EXPAND_PADDING);

  // Connect the signal handlers:
  m_ButtonQuit.signal_clicked().connect( sigc::mem_fun(*this,
              &IdleExample::on_button_clicked) );

  // formatting drive c in timeout signal handler - called once every 50ms
  Glib::signal_timeout().connect( sigc::mem_fun(*this, &IdleExample::on_timer),
          50 );

  // formatting drive d in idle signal handler - called as quickly as possible
  Glib::signal_idle().connect( sigc::mem_fun(*this, &IdleExample::on_idle) );

  show_all_children();
}


void IdleExample::on_button_clicked()
{
  hide();
}

// this timer callback function is executed once every 50ms (set in connection
// above).  Use timeouts when speed is not critical. (ie periodically updating
// something).
bool IdleExample::on_timer()
{
  double value = m_ProgressBar_c.get_fraction();

  // Update progressbar 1/500th each time:
  m_ProgressBar_c.set_fraction(value + 0.002);
 
  return value < 0.99;  // return false when done
}


// This idle callback function is executed as often as possible, hence it is
// ideal for processing intensive tasks.
bool IdleExample::on_idle()
{
  double value = m_ProgressBar_d.get_fraction();

  // Update progressbar 1/5000th each time:
  m_ProgressBar_d.set_fraction(value + 0.0002);

  return value < 0.99;  // return false when done
}