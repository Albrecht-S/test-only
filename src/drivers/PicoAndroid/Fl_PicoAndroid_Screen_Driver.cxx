//
// "$Id: Fl_PicoAndroid_Screen_Driver.cxx 11253 2016-03-01 00:54:21Z matt $"
//
// Definition of Android Screen interface based on Pico
//
// Copyright 1998-2016 by Bill Spitzak and others.
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


#include "../../config_lib.h"
#include "Fl_PicoAndroid_Screen_Driver.H"

#include <FL/Fl_Window_Driver.H>

//#define __APPLE__
//#include <SDL2/SDL.h>
//#undef __APPLE__


Fl_Screen_Driver* Fl_Screen_Driver::newScreenDriver()
{
  return new Fl_PicoAndroid_Screen_Driver();
}


Fl_PicoAndroid_Screen_Driver::Fl_PicoAndroid_Screen_Driver()
{
}

Fl_PicoAndroid_Screen_Driver::~Fl_PicoAndroid_Screen_Driver()
{
}


double Fl_PicoAndroid_Screen_Driver::wait(double time_to_wait)
{
  Fl::flush();
//  SDL_Event e;
//  if (SDL_PollEvent(&e)) {
//    switch (e.type) {
//      case SDL_QUIT:
//        exit(0);
//      case SDL_WINDOWEVENT_EXPOSED:
//      case SDL_WINDOWEVENT_SHOWN:
//      { // not happening!
//        //event->window.windowID
//        Fl_Window *window = Fl::first_window();
//        if ( !window ) break;;
//        Fl_X *i = Fl_X::i(Fl::first_window());
//        i->wait_for_expose = 0;
//
//        if ( i->region ) {
//          XDestroyRegion(i->region);
//          i->region = 0;
//        }
//        window->clear_damage(FL_DAMAGE_ALL);
//        i->flush();
//        window->clear_damage();
//        Fl_X::first->wait_for_expose = 0;
//      }
//        break;
//
//    }
//  }
  return 0.0;
}



#include <FL/x.H>
#include <FL/Fl.H>
#include <FL/Fl_Image_Surface.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Graphics_Driver.H>

/*
 * The following code should not be here! 
 * All this must be refactored into the driver system!
 */

/*

 The following symbols are not found if we naively compile the core modules and
 no specific platform implementations. This list is a hint at all the functions
 and methods that probably need to be refactored into the driver system.

 Undefined symbols for architecture x86_64:
 */

void fl_set_spot(int, int, int, int, int, int, Fl_Window*) { }
void fl_reset_spot() { }
const char *fl_filename_name(char const*) { return 0; }
void fl_clipboard_notify_change() { }

//Fl_Screen_Driver *Fl_Screen_Driver::newScreenDriver() { return 0; }
//Fl_Graphics_Driver *Fl_Graphics_Driver::newMainGraphicsDriver() { return 0; }
void Fl_Graphics_Driver::global_gc() { }
int Fl::dnd() { return 0; }
void Fl::copy(char const*, int, int, char const*) { }
void Fl::paste(Fl_Widget&, int, char const*) { }
void Fl::get_mouse(int&, int&) { }
void Fl::set_color(unsigned int, unsigned int) { }
int Fl_X::set_cursor(Fl_Cursor) { return 0; }
int Fl_X::set_cursor(Fl_RGB_Image const*, int, int) { return 0; }
void Fl_X::set_default_icons(Fl_RGB_Image const**, int) { }

void Fl_X::flush()
{
  w->driver()->flush();
}

void Fl_X::set_icons() { }
void Fl_Window::size_range_() { }
void Fl_Window::fullscreen_x() { }

void Fl_Window::make_current()
{
  fl_window = i->xid;
  current_ = this;
}

void Fl_Window::fullscreen_off_x(int, int, int, int) { }

Window fl_xid(const Fl_Window* w)
{
  Fl_X *temp = Fl_X::i(w);
  return temp ? temp->xid : 0;
}

void Fl_Window::show() {
  if (!shown()) {
    Fl_X::make(this);
  }
}

Fl_X* Fl_X::make(Fl_Window *w)
{
  return w->driver()->makeWindow();
}

void Fl_Window::label(char const*, char const*) { }
void Fl_Window::resize(int, int, int, int) { }
Fl_Window *Fl_Window::current_;
char fl_show_iconic;
Window fl_window;
//void Fl_Image_Surface::translate(int x, int y) { }
//void Fl_Image_Surface::untranslate() { }

void Fl::add_fd(int, void (*)(int, void*), void*)
{
}

void Fl::remove_fd(int)
{
}

int Fl_Window::decorated_h()
{
  return h();
}

int Fl_Window::decorated_w()
{
  return w();
}


//
// End of "$Id: Fl_PicoAndroid_Screen_Driver.cxx 11253 2016-03-01 00:54:21Z matt $".
//

