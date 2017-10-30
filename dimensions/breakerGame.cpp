#include <cairomm/context.h>
#include <glibmm/main.h>
#include <gtkmm.h>
#include <stdio.h>
#include "breakerGame.h"

bool breakerGame::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
  Gtk::Allocation allocation = get_allocation();
  width = allocation.get_width();
  height = allocation.get_height();
  playerPosX =  width/2-width/10;
  playerPosY = height-30;
  //tamano de los bloques
  const int rectangle_width = width / 10;
  const int rectangle_height = height / 20;

  // Fondo
  cr->set_source_rgb(0.0, 0.0, 0.0);
  draw_rectangle(cr, 0, 0, width, height);

  // bloques
  cr->set_source_rgb(1.0, 0, 0);
  y = 8;
  x = 8;
  for(int i = 0; i<4;i++){
    for (int j = 0; j < 8; j++) {
      if(mat[i][j]){
        draw_rectangle(cr, x, y, rectangle_width, rectangle_height);
        x+=rectangle_width+3;
      }
    }
    x = 8;
    y+=rectangle_height+3;
  }
  //jugador
  cr->set_source_rgb(1.0, 1.0, 1.0);
  draw_rectangle(cr, playerPosX, playerPosY, width/5, height/25);
  //bola
  if(ballPosX>=width || ballPosX<=0){
    moveX*=-1;
  }
  if(ballPosY>=height || ballPosY<=0){
    moveY*=-1;
  }
  if(ballPosX <= playerPosX+(width/5)
  && ballPosX >= playerPosX
  && ballPosY >= playerPosY
  && ballPosY <= playerPosY+height/50){
    //printf("Limite de jugador %f %i\n",ballPosY, playerPosY);
    moveY*=-1;
  }
  cr->arc(ballPosX, ballPosY, width/50, 0, 2 * M_PI);
  cr->fill();

  return true;
}

bool breakerGame::on_key_press_event(GdkEventKey* key_event)
{
  if(key_event->keyval == GDK_KEY_Left)
  {
    playerPosX-=10;
    printf("asdasdasd\n");
    return true;
  }
  return false;
}
bool breakerGame::on_timeout()
{
    ballPosX+=moveX;
    ballPosY+=moveY;
    auto win = get_window();
    if (win)
    {
        Gdk::Rectangle r(0, 0, get_allocation().get_width(),
                get_allocation().get_height());
        win->invalidate_rect(r, false);
    }
    return true;
}

void breakerGame::draw_rectangle(const Cairo::RefPtr<Cairo::Context>& cr, int x, int y, int width, int height)
{
  cr->rectangle(x, y, width, height);
  cr->fill();
}

void breakerGame::draw_text(const Cairo::RefPtr<Cairo::Context>& cr,
                       int rectangle_width, int rectangle_height)
{
  // http://developer.gnome.org/pangomm/unstable/classPango_1_1FontDescription.html
  Pango::FontDescription font;

  font.set_family("Monospace");
  font.set_weight(Pango::WEIGHT_BOLD);

  // http://developer.gnome.org/pangomm/unstable/classPango_1_1Layout.html
  auto layout = create_pango_layout("Hi there!");

  layout->set_font_description(font);

  int text_width;
  int text_height;

  //get the text dimensions (it updates the variables -- by reference)
  layout->get_pixel_size(text_width, text_height);

  // Position the text in the middle
  cr->move_to((rectangle_width-text_width)/2, (rectangle_height-text_height)/2);

  layout->show_in_cairo_context(cr);
}


breakerGame::breakerGame()
{
  Glib::signal_timeout().connect( sigc::mem_fun(*this, &breakerGame::on_timeout), 10 );
  //signal_key_press_event().connect(mem_ptr(&breakerGame::on_key_press_event));
  this->add_events( Gdk::KEY_PRESS_MASK );

  this->signal_key_press_event().connect ( sigc::mem_fun(*this,&breakerGame::on_key_press_event));

  for(int i = 0; i < 4 ; i++){
    for(int j = 0; j<8; j++){
      mat[i][j] = 1;
    }
  }
  x = 8;
  y = 8;
  ballPosX = 100;
  ballPosY = 150;
  moveX = 0.3;
  moveY = 0.3;
}

breakerGame::~breakerGame()
{
}
