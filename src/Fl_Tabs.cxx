// "$Id: Fl_Tabs.cxx,v 1.75 2004/12/12 22:23:24 spitzak Exp $"
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

/*! \class fltk::TabGroup

This is the "file card tabs" interface to allow you to put lots and
lots of buttons and switches in a panel. This first appeared in
NeXTStep, but is best known from Windows control panesl. FLTK's
version draws in a style more reminiscent of NeXT or PageMaker, and
avoids the confusing multiple-lines of Windows by drawing overlapping
tabs.

\image html tabs.gif

Each child widget is a card, and it's label() is printed on the card
tab (including the label font and style).  The color() of the child is
used to color the tab as well.

The size of the tabs is controlled by the bounding box of the children
(there should be some space between the children and the edge of this
widget). If there is a larger gap on the bottom than the top, the tabs
are placed "inverted" along the bottom. It is easiest to lay this out
in fluid, using the fluid browser to select each child group and
resize them until the tabs look the way you want them to.

Clicking the tab makes that child visible() and hides all the
other children. If the widget with the focus does not consume them,
the ctrl+tab and ctrl+shift+tab keys will also switch tabs. The
user can also navigate the focus to the tabs and change them with
the arrow keys.

The callback() of the TabGroup widget is called when
the user changes the visible tab, and SHOW and HIDE events are passed
to the children.
*/

#include <config.h>
#include <fltk/TabGroup.h>
#include <fltk/events.h>
#include <fltk/damage.h>
#include <fltk/layout.h>
#include <fltk/Box.h>
#include <fltk/draw.h>
#include <fltk/Tooltip.h>
using namespace fltk;

#define BORDER 10
#define TABSLOPE 5
#define EXTRASPACE 5

// return the left edges of each tab (plus a fake left edge for a tab
// past the right-hand one).  These position are actually of the left
// edge of the slope.  They are either seperated by the correct distance
// or by TABSLOPE or by zero.
// Return value is the index of the selected item.

int TabGroup::tab_positions(int* p, int* w) {
  int selected = 0;
  int i;
  int numchildren = children();
  int width = 0;
  p[0] = w[0] = 0;
  for (i=0; i<numchildren; i++) {
    Widget* o = child(i);
    if (o->visible()) selected = i;
    int wt = 300; int ht = 300; // rather arbitrary choice for max size
    o->measure_label(wt, ht);
    w[i] = wt+TABSLOPE+EXTRASPACE;
    //if (2*TABSLOPE > w[i]) w[i] = 2*TABSLOPE;
    width += w[i];
    p[i+1] = width;
  }
  int r = this->w()-TABSLOPE-1;
  if (width <= r) return selected;
  // uh oh, they are too big, we must move them:
  // special case when the selected tab itself is too big, make it fill width:
  if (w[selected] >= r) {
    w[selected] = r;
    for (i = 0; i <= selected; i++) p[i] = 0;
    for (i = selected+1; i <= numchildren; i++) p[i] = r;
    return selected;
  }
  int w2[128];
  for (i = 0; i < numchildren; i++) w2[i] = w[i];
  i = numchildren-1;
  int j = 0;
  int minsize = TABSLOPE;
  bool right = true;
  while (width > r) {
    int n; // which one to shrink
    if (j < selected && (!right || i <= selected)) { // shrink a left one
      n = j++;
      right = true;
    } else if (i > selected) { // shrink a right one
      n = i--;
      right = false;
    } else { // no more space, start making them zero
      minsize = 0;
      i = numchildren-1;
      j = 0;
      right = true;
      continue;
    }
    width -= w2[n]-minsize;
    w2[n] = minsize;
    if (width < r) {
      w2[n] = r-width+minsize;
      width = r;
      break;
    }
  }
  // re-sum the positions:
  width = 0;
  for (i = 0; i < numchildren; i++) {
    width += w2[i];
    p[i+1] = width;
  }
  return selected;
}

// return space needed for tabs.  Negative to put them on the bottom:
int TabGroup::tab_height() {
  int H = h();
  int H2 = 0;
  int numchildren = children();
  for (int i=0; i < numchildren; i++) {
    Widget* o = child(i);
    if (o->y() < H) H = o->y();
    if (o->y()+o->h() > H2) H2 = o->y()+o->h();
  }
  H2 = h()-H2;
  int dx=0; int dy=0; int dw=0; int dh=0; box()->inset(dx,dy,dw,dh);
  if (H2 > H) {
    H = H2-dy;
    return (H <= 0) ? 0 : -H-1;
  } else {
    H = H-dy;
    return (H <= 0) ? 0 : H;
  }
}

/*! Return the child index that would be selected by a click at the
  given mouse position. Returns -1 if the mouse position is not
  in a tab.
*/
int TabGroup::which(int event_x, int event_y) {
  int H = tab_height();
  if (H < 0) {
    if (event_y > h() || event_y < h()+H) return -1;
  } else {
    if (event_y > H || event_y < 0) return -1;
  }
  if (event_x < 0) return -1;
  int p[128], w[128];
  int selected = tab_positions(p, w);
  int d = (event_y-(H>=0?0:h()))*TABSLOPE/H;
  for (int i=0; i<children(); i++) {
    if (event_x < p[i+1]+(i<selected ? TABSLOPE-d : d)) return i;
  }
  return -1;
}

int TabGroup::handle(int event) {

  int i = value();
  Widget* selected = i >= 0 ? child(i) : 0;
  int backwards = 0;

  switch (event) {

  case FOCUS_CHANGE:
    // this is called to indicate that some child got the focus
    if (focus_index() < 0) redraw(DAMAGE_VALUE);
    focus_index(fltk::focus() == this ? -1 : 0);
    return true;

  case FOCUS:
    if (fltk::focus() == this) goto GOTO_TABS;
    switch (navigation_key()) {
    case LeftKey:
    case UpKey:
      if (tab_height() < 0) goto GOTO_TABS; else goto GOTO_CONTENTS;
    case RightKey:
    case DownKey:
      if (tab_height() < 0) goto GOTO_CONTENTS; else goto GOTO_TABS;
    default:
      if (focus_index() < 0) goto GOTO_TABS;
    GOTO_CONTENTS:
      // Try to give the contents the focus. Also preserve a return value
      // of 2 (which indicates the contents have a text field):
      if (selected) {
	int n = selected->handle(FOCUS);
	if (n) {
	  if (!selected->contains(fltk::focus())) fltk::focus(selected);
	  return n;
	}
      }
    GOTO_TABS:
      focus_index(-1);
      redraw(DAMAGE_VALUE);
      return true;
    }

  case UNFOCUS:
    if (focus_index() < 0) redraw(DAMAGE_VALUE);
    return 1;

  // handle mouse events in the tabs:
  case PUSH: {
    int H = tab_height();
    if (H >= 0) {
      if (event_y() > H) break;
    } else {
      if (event_y() < h()+H) break;
    }}
  case DRAG:
  case RELEASE:
    i = which(event_x(), event_y());
    if (event == RELEASE && !pushed()) {
      push(0);
      if (i >= 0 && value(i)) do_callback();
    } else {
      push(i >= 0 ? child(i) : 0);
    }
    return 1;

  // handle pointing at the tabs to bring up tooltip:
  case ENTER:
  case MOVE: {
    int H = tab_height();
    if (H >= 0) {
      if (event_y() > H) break;
    } else {
      if (event_y() < h()+H) break;
    }
    i = which(event_x(), event_y());
    if (i >= 0)
      Tooltip::enter(this, 0, H<0?h()+H:0, w(), H<0?-H:H,
		     child(i)->tooltip());
    return 1;}

  case KEY:
    if (fltk::focus()==this) {
      switch (event_key()) {
      case SpaceKey:
      case RightKey:
	goto MOVE;
      case BackSpaceKey:
      case LeftKey:
	backwards = 1;
      MOVE:
	if (backwards) {i = i ? i-1 : children()-1;}
	else {i++; if (i >= children()) i = 0;}
	if (value(i)) do_callback();
	return 1;
      }
    }
    switch (navigation_key()) {
    case RightKey:
    case DownKey:
      if (tab_height()<0) goto UP_CASE;
    DOWN_CASE:
      if (focus_index() < 0 && selected) return selected->take_focus();
      else return 0;
    case LeftKey:
    case UpKey:
      if (tab_height()<0) goto DOWN_CASE;
    UP_CASE:
      if (focus_index() >= 0) {fltk::focus(this); return 1;}
      else return 0;
    }
    // fall through to SHORTCUT case:
  case SHORTCUT:
    if (event_key() == TabKey && event_state(CTRL)) {
      backwards = event_state(SHIFT);
      goto MOVE;
    }
    break;

  }

  if (selected) return selected->send(event);
  return 0;
}

static Widget* push_ = 0;

int TabGroup::push(Widget *o) {
  if (push_ == o) return 0;
  if (push_ && !push_->visible() || o && !o->visible())
    redraw(DAMAGE_VALUE);
  push_ = o;
  return 1;
}

/*! Return the index of the first visible() child, which is normally
  the one the user selected.

  This will automatically force a single child to be visible() if
  more than one is, or if none are. If more than one is visible all
  except the first is hidden. If none are, the last one is made
  visible. The resulting visible child's index is returned. This behavior
  allows new TabGroups to be created with all children visible, and
  allows children to be deleted, moved to other groups, and
  show()/hide() called on them without the display ever looking wrong
  to the user.

  If there are no children then -1 is returned.
*/
int TabGroup::value() {
  int ret = -1;;
  int numchildren = children();
  for (int i=0; i < numchildren; i++) {
    Widget* o = child(i);
    if (ret >= 0) o->hide();
    else if (o->visible()) ret = i;
    else if (i==numchildren-1) {o->show(); ret = i;}
  }
  return ret;
}

/*! Return child(value()), or return null if no children. */
Widget* TabGroup::selected_child() {
  int i = value();
  return i >= 0 ? child(i) : 0;
}

/*! Switch so index n is selected. If n is less than zero selects
  zero, if n is greater than the children it selects the last one.
  Returns true if this is a different child than last time. Does
  not do the callback().
*/
bool TabGroup::value(int n) {
  if (n < 0) n = 0;
  if (n >= children()) {
    if (!children()) return false;
    n = children()-1;
  }
  return selected_child(child(n));
}

/*! Switch to this child widget, or to a child that contains() this
  widget.  Returns true if this is a different selection than
  before. Does not do the callback(). If the widget is null or not a
  descendent of this, the last child is selected.
*/
bool TabGroup::selected_child(Widget *newvalue) {
  int setfocus = !focused() && contains(fltk::focus());
  int numchildren = children();
  for (int i=0; i < numchildren; i++) {
    Widget* o = child(i);
    if (o == newvalue) {
      if (o->visible()) return 0; // no change
      o->show();
      if (setfocus) o->take_focus();
    } else {
      o->hide();
    }
  }
  return 1;
}

enum {LEFT, RIGHT, SELECTED};

extern Widget* fl_did_clipping;

static int H;
static int p[128];
void TabGroup::draw() {
  Widget *v = selected_child();

  H = tab_height();
  if (damage() & DAMAGE_ALL) { // redraw the entire thing:
    push_clip(0, 0, w(), h());
#if NO_CLIP_OUT
    draw_background();
#endif
    if (v) draw_child(*v);
#if !NO_CLIP_OUT
    draw_background();
#endif
    pop_clip();
  } else { // redraw the child
    if (v) update_child(*v);
  }

  // draw the tabs if needed:
  if (damage() & (DAMAGE_VALUE|DAMAGE_ALL)) {
    int w[128];
    int selected = tab_positions(p,w);
    int i;

    for (i=0; i<selected; i++)
      draw_tab(p[i], p[i+1], w[i], H, child(i), LEFT);
    for (i=children()-1; i > selected; i--)
      draw_tab(p[i], p[i+1], w[i], H, child(i), RIGHT);
    if (v) {
      i = selected;
      draw_tab(p[i], p[i+1], w[i], H, child(i), SELECTED);
    } else {
      // draw the edge when no selection:
      setcolor(H >= 0 ? GRAY99 : GRAY33);
      int b = H >= 0 ? H : h()+H;
      drawline(0, b, this->w(), b);
    }

  }
#if !NO_CLIP_OUT
  if (damage() & DAMAGE_EXPOSE) {
    clip_out(0, H>=0 ? 0 : h()+H, p[children()]+TABSLOPE, (H>=0?H:-H));
    clip_out(0, H>0 ? H : 0, this->w(), h()-(H>=0?H:-H-1));
    fl_did_clipping = this;
  }
#endif
}

void TabGroup::draw_tab(int x1, int x2, int W, int H, Widget* o, int what) {
  if (x2 <= x1) return; // ignore ones shrunk to zero width
  bool drawlabel = true;
  if (x2 < x1+W) {
    if (x2 <= x1+TABSLOPE) drawlabel = false;
    if (what == LEFT) {
      if (x1+W < x2+TABSLOPE) x2 = x1+W;
      else x2 += TABSLOPE;
    } else {
      if (x1+W < x2+TABSLOPE) x1 = x2-W;
      else x1 -= TABSLOPE;
    }
  }
  int sel = (what == SELECTED);
  setcolor(o->color());
  if (H >= 0) {
    newpath();
    addvertex(x1, H+sel);
    addvertex(x1+TABSLOPE, 0);
    addvertex(x2, 0);
    addvertex(x2+TABSLOPE, H+sel);
    fillpath();
    setcolor(!sel && o==push_ ? GRAY33 : GRAY99);
    drawline(x1, H, x1+TABSLOPE, 0);
    drawline(x1+TABSLOPE, 0, x2, 0);
    if (sel) {
      if (x1>0) drawline(0, H, x1, H);
      if (x2+TABSLOPE < w()-1) drawline(x2+TABSLOPE, H, w()-1, H);
    }
    setcolor(!sel && o==push_ ? GRAY99 : GRAY33);
    drawline(x2, 0, x2+TABSLOPE, H);
  } else {
    newpath();
    addvertex(x1, h()+H-sel);
    addvertex(x1+TABSLOPE, h());
    addvertex(x2, h());
    addvertex(x2+TABSLOPE, h()+H-sel);
    fillpath();
    setcolor(!sel && o==push_ ? GRAY99 : GRAY33);
    newpath();
    addvertex(x1+TABSLOPE, h()-1);
    addvertex(x2, h()-1);
    addvertex(x2+TABSLOPE, h()+H);
    strokepath();
    if (sel) {
      if (x1>0) drawline(0, h()+H, x1, h()+H);
      if (x2+TABSLOPE < w()-1) drawline(x2+TABSLOPE, h()+H, w()-1, h()+H);
    }
    setcolor(!sel && o==push_ ? GRAY33 : GRAY99);
    drawline(x1, h()+H, x1+TABSLOPE, h()-1);
  }
  if (drawlabel) {
    int x = (what==LEFT ? x1 : x2-W)+(TABSLOPE+EXTRASPACE/2);
    int w = W-(TABSLOPE+EXTRASPACE/2);
    int y,h;
    if (H<0) {
      y = this->h()+H-1;
      h = -H-1;
    } else {
      y = 2;
      h = H-1;
    }
    o->draw_label(x, y, w, h, o->style(), ALIGN_CENTER);
    if (sel && focused()) focusbox()->draw(x, y, w, h, style(), FOCUSED);
  }
}

static void revert(Style* s) {
  s->box_ = THIN_UP_BOX;
  s->color_ = GRAY75;
}
static NamedStyle style("Tabs", revert, &TabGroup::default_style);
/*! The default style has a gray color() and the box() is set to 
  THIN_UP_BOX. The box() is used to draw the edge around the cards,
  including the top edge, but the tab itself is designed only to
  match THIN_UP_BOX. You can also use FLAT_BOX and it will look
  correct if the tabs fill the entire width of a window or parent
  box.
*/
NamedStyle* TabGroup::default_style = &::style;

/*! Creates a new TabGroup widget using the given position, size, and
label string. Use add(widget) to add each child. Each child is
probably an fltk::Group widget containing the actual widgets the user
sees. The children should be sized to stay away from the top or bottom
edge of the <TT>fltk::Tabs</TT>, which is where the tabs are drawn.
*/
TabGroup::TabGroup(int X,int Y,int W, int H, const char *l)
  : Group(X,Y,W,H,l)
{
  style(default_style);
  focus_index(0);
}

// End of "$Id: Fl_Tabs.cxx,v 1.75 2004/12/12 22:23:24 spitzak Exp $".
