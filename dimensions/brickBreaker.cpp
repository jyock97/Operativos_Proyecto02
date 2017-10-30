#include <gtkmm/application.h>
#include <gtkmm/window.h>
#include "breakerGame.h"



int main(int argc, char** argv)
{
   auto app = Gtk::Application::create(argc, argv, "org.gtkmm.example");

   Gtk::Window win;
   win.set_default_size(200, 200);
   win.set_title("BRICK BREAKER");
   breakerGame game;
   win.add(game);
   game.show();
   return app->run(win);
}
