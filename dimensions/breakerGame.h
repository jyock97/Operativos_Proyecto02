#ifndef GTKMM_EXAMPLE_MYAREA_H
#define GTKMM_EXAMPLE_MYAREA_H

#include <gtkmm/drawingarea.h>
#include <gtkmm/application.h>
class breakerGame : public Gtk::DrawingArea
{
public:
  breakerGame();
  virtual ~breakerGame();

protected:
  //Override default signal handler:
  bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;
  bool on_key_press_event(GdkEventKey* event) override;
  bool on_timeout();

private:
  int mat[4][8];
  int x, y;
  int width;
  int height;
  int playerPosX, playerPosY;
  double ballPosX, ballPosY;
  double moveX, moveY;
  void draw_rectangle(const Cairo::RefPtr<Cairo::Context>& cr, int x, int y, int width, int height);
  void draw_text(const Cairo::RefPtr<Cairo::Context>& cr, int rectangle_width, int rectangle_height);
};

#endif // GTKMM_EXAMPLE_MYAREA_H
