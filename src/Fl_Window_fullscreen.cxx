//
// "$Id: Fl_Window_fullscreen.cxx,v 1.7 1999/10/03 06:31:40 bill Exp $"
//
// Fullscreen window support for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-1999 by Bill Spitzak and others.
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
// Please report all bugs and problems to "fltk-bugs@easysw.com".
//

#include <FL/Fl.H>

#ifndef WIN32
#include <FL/x.H>
extern Atom fl_motif_wm_hints; // in Fl_x.cxx
#endif

void Fl_Window::fullscreen() {
#ifdef WIN32
  // something must be done so that the taskbar is hidden...
#else
  // Irix 4DWM will work if we use the motif_wm_hints property to turn
  // the border off.  Most other window managers will ignore this and
  // leave the border on.  Newer window managers will position the
  // window correctly even if the border is on, but far too many of
  // them will insist on moving it down&right by the border thickness...
  if (i) {
    // see the file /usr/include/X11/Xm/MwmUtil.h:
    long prop[5] = {2, 1, 0, 0, 0};
    XChangeProperty(fl_display, fl_xid(this),
		    fl_motif_wm_hints, fl_motif_wm_hints,
		    32, 0, (unsigned char *)prop, 5);
  }
#endif
  if (!x()) x(1); // force it to call XResizeWindow()
  resize(0,0,Fl::w(),Fl::h());
}

void Fl_Window::fullscreen_off(int X,int Y,int W,int H) {
  resize(X,Y,W,H);
#ifndef WIN32
  if (i) i->sendxjunk(); // makes the border turn back on
#endif
}

//
// End of "$Id: Fl_Window_fullscreen.cxx,v 1.7 1999/10/03 06:31:40 bill Exp $".
//
