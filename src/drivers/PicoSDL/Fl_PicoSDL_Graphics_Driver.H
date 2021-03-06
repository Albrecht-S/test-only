
//
// "$Id$"
//
// Definition of the Pico minimal SDL graphics driver
// for the Fast Light Tool Kit (FLTK).
//
// Copyright 2010-2016 by Bill Spitzak and others.
//
// This library is free software. Distribution and use rights are outlined in
// the file "COPYING" which should have been included with this file.  If this
// file is missing or damaged, see the license at:
//
//     http://www.fltk.org/COPYING.php
//
// Please report all bugs and problems on the following page:
//
//     http://www.fltk.org/str.php
//

/**
 \file Fl_PicoSDL_Graphics_Driver.H
 \brief Definition of Pico minimal graphics driver.
 */

#ifndef FL_PICOSDL_GRAPHICS_DRIVER_H
#define FL_PICOSDL_GRAPHICS_DRIVER_H

#include "../Pico/Fl_Pico_Graphics_Driver.H"


/**
 \brief The Pico minimal SDL graphics class.
 *
 This class is implemented as a base class for minimal core SDL drivers.
 */
class Fl_PicoSDL_Graphics_Driver : public Fl_Pico_Graphics_Driver {
protected:
  //  CGContextRef gc_;
public:
  //  virtual int has_feature(driver_feature mask) { return mask & NATIVE; }
  //  virtual void *gc() {return gc_;}
  //  char can_do_alpha_blending();
  //
  //  // --- bitmap stuff
  //  Fl_Bitmask create_bitmask(int w, int h, const uchar *array);
  //  void delete_bitmask(Fl_Bitmask bm);
  //  void draw(Fl_Pixmap *pxm, int XP, int YP, int WP, int HP, int cx, int cy);
  //  void draw(Fl_Bitmap *pxm, int XP, int YP, int WP, int HP, int cx, int cy);
  //  void draw(Fl_RGB_Image *img, int XP, int YP, int WP, int HP, int cx, int cy);
  //  int draw_scaled(Fl_Image *img, int XP, int YP, int WP, int HP);
  //  void draw_image(const uchar* buf, int X,int Y,int W,int H, int D=3, int L=0);
  //  void draw_image(Fl_Draw_Image_Cb cb, void* data, int X,int Y,int W,int H, int D=3);
  //  void draw_image_mono(const uchar* buf, int X,int Y,int W,int H, int D=1, int L=0);
  //  void draw_image_mono(Fl_Draw_Image_Cb cb, void* data, int X,int Y,int W,int H, int D=1);
  //  fl_uintptr_t cache(Fl_Pixmap *img, int w, int h, const char *const*array);
  //  fl_uintptr_t cache(Fl_Bitmap *img, int w, int h, const uchar *array);
  //  void uncache(Fl_RGB_Image *img, fl_uintptr_t &id_, fl_uintptr_t &mask_);
  //  void copy_offscreen(int x, int y, int w, int h, Fl_Offscreen pixmap, int srcx, int srcy);
  //  void draw_CGImage(CGImageRef cgimg, int x, int y, int w, int h, int srcx, int srcy, int sw, int sh);
  //protected:
  //  // --- implementation is in src/fl_rect.cxx which includes src/cfg_gfx/quartz_rect.cxx
  void point(int x, int y);
  //  void rect(int x, int y, int w, int h);
  void rectf(int x, int y, int w, int h);
  void line(int x, int y, int x1, int y1);
  //  void line(int x, int y, int x1, int y1, int x2, int y2);
  void xyline(int x, int y, int x1) { line(x, y, x1, y); }
  //  void xyline(int x, int y, int x1, int y2);
  //  void xyline(int x, int y, int x1, int y2, int x3);
  void yxline(int x, int y, int y1) { line(x, y, x, y1); }
  //  void yxline(int x, int y, int y1, int x2);
  //  void yxline(int x, int y, int y1, int x2, int y3);
  //  void loop(int x0, int y0, int x1, int y1, int x2, int y2);
  //  void loop(int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3);
  //  void polygon(int x0, int y0, int x1, int y1, int x2, int y2);
  //  void polygon(int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3);
  //  // --- clipping
  //  void push_clip(int x, int y, int w, int h);
  //  int clip_box(int x, int y, int w, int h, int &X, int &Y, int &W, int &H);
  //  int not_clipped(int x, int y, int w, int h);
  //  void push_no_clip();
  //  void pop_clip();
  //  void restore_clip();
  //  // --- implementation is in src/fl_vertex.cxx which includes src/cfg_gfx/xxx_rect.cxx
  //  void begin_complex_polygon();
  //  void transformed_vertex(double xf, double yf);
  //  void vertex(double x, double y);
  //  void end_points();
  //  void end_line();
  //  void end_loop();
  //  void end_polygon();
  //  void end_complex_polygon();
  //  void gap();
  //  void circle(double x, double y, double r);
  //  // --- implementation is in src/fl_arc.cxx which includes src/cfg_gfx/xxx_arc.cxx if needed
  //  // using void Fl_Graphics_Driver::arc(double x, double y, double r, double start, double end);
  //  // --- implementation is in src/fl_arci.cxx which includes src/cfg_gfx/xxx_arci.cxx
  //  void arc(int x, int y, int w, int h, double a1, double a2);
  //  void pie(int x, int y, int w, int h, double a1, double a2);
  //  // --- implementation is in src/fl_line_style.cxx which includes src/cfg_gfx/xxx_line_style.cxx
  //  void line_style(int style, int width=0, char* dashes=0);
  //  // --- implementation is in src/fl_color.cxx which includes src/cfg_gfx/xxx_color.cxx
  //  void color(Fl_Color c);
  //  Fl_Color color() { return color_; }
  //  void color(uchar r, uchar g, uchar b);
  //  // --- implementation is in src/fl_font.cxx which includes src/cfg_gfx/xxx_font.cxx
  //  void draw(const char *str, int n, int x, int y);
  //  void draw(const char *str, int n, float x, float y);
  //  void draw(int angle, const char *str, int n, int x, int y);
  //  void rtl_draw(const char *str, int n, int x, int y);
  //  void font(Fl_Font face, Fl_Fontsize fsize);
  //  double width(const char *str, int n);
  //  double width(unsigned int c);
  //  void text_extents(const char*, int n, int& dx, int& dy, int& w, int& h);
  //  int height();
  //  int descent();
};

#endif // FL_PICOSDL_GRAPHICS_DRIVER_H

//
// End of "$Id$".
//
