//
// "$Id$"
//
// Button widget for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2004 by Bill Spitzak and others.
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Library General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA.
//
// Please report all bugs and problems to "fltk-bugs@fltk.org".
//

#include <FL/Fl.H>
#include <FL/Fl_Tool_Button.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Window.H>

#include <FL/Fl_Style.H>
#include <FL/Fl_Style_List.H>


// There are a lot of subclasses, named Fl_*_Button.  Some of
// them are implemented by setting the type() value and testing it
// here.  This includes Fl_Radio_Button and Fl_Toggle_Button

int Fl_Button::value(int v) {
  v = v ? 1 : 0;
  oldval = v;
  clear_changed();
  if (value_ != v) {
    value_ = v;
    redraw();
    return 1;
  } else {
    return 0;
  }
}

void Fl_Button::setonly() { // set this radio button on, turn others off
  value(1);
  Fl_Group* g = (Fl_Group*)parent();
  Fl_Widget*const* a = g->array();
  for (int i = g->children(); i--;) {
    Fl_Widget* o = *a++;
    if (o != this && o->type()==FL_RADIO_BUTTON) ((Fl_Button*)o)->value(0);
  }
}

void Fl_Button::draw() {
  if (type() == FL_HIDDEN_BUTTON) return;
  Fl_Color col = value() ? selection_color() : color();
//if (col == FL_GRAY && Fl::belowmouse()==this) col = FL_LIGHT1;
  draw_box(value() ? (down_box()?down_box():fl_down(box())) : box(), col);
  draw_label();
  if (Fl::focus() == this) draw_focus();
}

int Fl_Button::handle(int event) {

  int newval;
  switch (event) {
  case FL_ENTER: highlight(); return 1;
  case FL_LEAVE: normal();
//  if ((value_?selection_color():color())==FL_GRAY) redraw();
    return 1;
  case FL_PUSH:
    if (Fl::visible_focus() && handle(FL_FOCUS)) Fl::focus(this);
  case FL_DRAG:
    if (Fl::event_inside(this)) {
      if (type() == FL_RADIO_BUTTON) newval = 1;
      else newval = !oldval;
    } else
      newval = oldval;
    if (newval != value_) {
      value_ = newval;
      set_changed();
      redraw();
      if (when() & FL_WHEN_CHANGED) do_callback();
    }
    return 1;
  case FL_RELEASE:
    if (value_ == oldval) {
      if (when() & FL_WHEN_NOT_CHANGED) do_callback();
      return 1;
    }
    set_changed();
    if (type() == FL_RADIO_BUTTON) setonly();
    else if (type() == FL_TOGGLE_BUTTON) oldval = value_;
    else {
      value(oldval);
      if (when() & FL_WHEN_CHANGED) do_callback();
    }
    if (when() & FL_WHEN_RELEASE) do_callback();
    return 1;
  case FL_SHORTCUT:
    if (!(shortcut() ?
	  Fl::test_shortcut(shortcut()) : test_shortcut())) return 0;
    
    if (Fl::visible_focus() && handle(FL_FOCUS)) Fl::focus(this);

    if (type() == FL_RADIO_BUTTON && !value_) {
      setonly();
      set_changed();
      if (when() & FL_WHEN_CHANGED) do_callback();
    } else if (type() == FL_TOGGLE_BUTTON) {
      value(!value());
      set_changed();
      if (when() & FL_WHEN_CHANGED) do_callback();
    } else if (when() & FL_WHEN_RELEASE) do_callback();
    return 1;
  case FL_FOCUS :
  case FL_UNFOCUS :
    if (Fl::visible_focus()) {
      if (box() == FL_NO_BOX) {
	// Widgets with the FL_NO_BOX boxtype need a parent to
	// redraw, since it is responsible for redrawing the
	// background...
	int X = x() > 0 ? x() - 1 : 0;
	int Y = y() > 0 ? y() - 1 : 0;
	if (window()) window()->damage(FL_DAMAGE_ALL, X, Y, w() + 2, h() + 2);
      } else redraw();
      return 1;
    } else return 0;
  case FL_KEYBOARD :
    if (Fl::focus() == this && Fl::event_key() == ' ' &&
        !(Fl::event_state() & (FL_SHIFT | FL_CTRL | FL_ALT | FL_META))) {
      set_changed();
      if (type() == FL_RADIO_BUTTON && !value_) {
	setonly();
	if (when() & FL_WHEN_CHANGED) do_callback();
      } else if (type() == FL_TOGGLE_BUTTON) {
	value(!value());
	if (when() & FL_WHEN_CHANGED) do_callback();
      }
      if (when() & FL_WHEN_RELEASE) do_callback();
      return 1;
    }
  default:
    return 0;
  }
}

Fl_Button::Fl_Button(int X, int Y, int W, int H, const char *l)
: Fl_Widget(X,Y,W,H,l) {
 
  style_ = Fl_Button::default_style();
  value_ = oldval = 0;
  shortcut_ = 0;
  set_flag(SHORTCUT_LABEL);
}

Fl_Boxtype Fl_Button::down_box() const{
  return ((Fl_Button::Style *)style_)->down_box();
}
void Fl_Button::down_box(Fl_Boxtype a) {
  dynamic_style();
  ((Fl_Button::Style *)style_)->down_box_ = a;
  ((Fl_Button::Style *)style_)->set_flag(Style::DOWN_BOX);
}


/////////////////  Fl_Button::Style ////////////////////////////

Fl_Button::Style::Style(Fl_Widget::Style * parent, unsigned mode):Fl_Widget::Style(parent,BASE){
  down_box_ = FL_NO_BOX;
  clear_flag(DOWN_BOX);
  init(parent,mode);
}



void Fl_Button::Style::update_(Fl_Button::Style *s1, Fl_Widget::Style * s, unsigned what){
  if(!s) return;
  Fl_Widget::Style::update_(s1, s, what);
  if(DOWN_BOX & ~(s->flags()) & what) ((Fl_Button::Style *)s)->down_box_ = s1->down_box_;
};

void Fl_Button::Style::down_box(Fl_Boxtype b) {
  down_box_= b;
  set_flag (DOWN_BOX);
  fl_update_styles(Style, down_box,DOWN_BOX,b);
}

Fl_Button::Style * Fl_Button::default_style(){
  static Fl_Button::Style * s = 0;
  if(!s){ // not yet initialized
    s = new Fl_Button::Style(Fl_Widget::default_style(), Fl_Widget::Style::NORMAL_HIGHLIGHT);
    s->box(FL_UP_BOX);
    s->down_box(FL_NO_BOX);
  }
  return s;
}

///////////////////////////////////////////////////////////////////////

//static fl_widget_value = 1;


void Fl_Button::draw_box_(Fl_Boxtype b, int X, int Y, int W, int H, Fl_Color c) const {
  if(!value()) clear_draw_box_flag(DRAW_BOX_VALUE);
  Fl_Widget::draw_box_(b, X, Y, W, H, c);
}

int Fl::draw_box_value() { return Fl_Widget::draw_box_flags() & Fl_Button::DRAW_BOX_VALUE; }


//
// End of "$Id$".
//
