//
// "$Id: fl_xft.cxx,v 1.4 2001/11/08 08:13:49 spitzak Exp $"
//
// Plugin file for FLTK
//
// Copyright 1999 Bill Spitzak and others.
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

// Plugin for fltk to use the Xft library for font rendering.  This enables
// your fonts to display anti-aliased and slow just like Windows!
//
// This plugin only works if
// 1. You have XFree86 4
// 2. You have the render extension
// 3. Your X device driver supports the render extension
// 4. You have libXft
// 5. Your libXft has freetype2 support compiled in
// 6. You have the freetype2 library
//
// Distributions that have XFree86 4.0.3 or later should have all of this...


#include <X11/Xlib.h>
#include <X11/Xft/Xft.h>

#include <fltk/Fl.h>
#include <fltk/Fl_Window.h>
#include <fltk/fl_draw.h>
#include <fltk/Fl_Font.h>
#include <fltk/x.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int min_antialias_size = 16;

Fl_FontSize *fl_fontsize;
#define current_font ((XftFont*)(fl_fontsize->font))

// Static variable for the default encoding:
const char *fl_encoding_ = "iso8859-1";

// Change the encoding in use now. This runs the font search again with
// the new encoding.
void fl_encoding(const char* f) {
  if (f != fl_encoding_) {
    fl_encoding_ = f;
    if (fl_font_) fl_font(fl_font_, fl_size_);
  }
}

static void
xft_font(Fl_Font font, unsigned size) {
  if (font == fl_font_ && size == fl_size_ &&
      !strcasecmp(fl_fontsize->encoding, fl_encoding_))
    return;

  fl_font_ = font; fl_size_ = size;

  Fl_FontSize* f;
  // search the fontsizes we have generated already
  for (f = font->first; f; f = f->next) {
    if (f->minsize == size && !strcasecmp(f->encoding, fl_encoding_))
      break;
  }

  if (!f) {
    f = new Fl_FontSize(font->name_);
    f->next = (Fl_FontSize *)(font->first);
    ((Fl_Font_*)font)->first = f;
  }

  fl_fontsize = f;
}

Fl_FontSize::Fl_FontSize(const char* name) {
  font = fl_font_renderer->load(name, fl_encoding_, fl_size_);
  encoding = fl_encoding_;
  minsize = maxsize = fl_size_;
#if HAVE_GL
  listbase = 0;
#endif
}

static void *
xft_font_load(const char *name, const char *encoding, int size) {
  int slant = XFT_SLANT_ROMAN;
  int weight = XFT_WEIGHT_MEDIUM;
  // may be efficient, but this is non-obvious
  switch (*name++) {
  case 'I': slant = XFT_SLANT_ITALIC; break;
  case 'P': slant = XFT_SLANT_ITALIC;
  case 'B': weight = XFT_WEIGHT_BOLD; break;
  case ' ': break;
  default: name--;
  }

  // this call is extremely slow...
  XftFont *font = XftFontOpen(fl_display, fl_screen,
                              XFT_FAMILY, XftTypeString, name,
                              XFT_WEIGHT, XftTypeInteger, weight,
                              XFT_SLANT, XftTypeInteger, slant,
//                              XFT_ENCODING, XftTypeString, encoding,
                              XFT_PIXEL_SIZE, XftTypeDouble, (double)size,
//                               XFT_ANTIALIAS, XftTypeBool, antialias,
                              0);

  return font;
}

static void
xft_font_unload(void *font) {
//printf("xft_font_unload(void *font = %p)\n", font);
  XftFontClose(fl_display, (XftFont *)font);
}

static int
xft_font_height() { return current_font->height; }

static int
xft_font_descent() { return current_font->descent; }

static int
xft_font_width(const char *str, int n) {
//printf("xft_font_width(const char *, int)\n");
  XGlyphInfo i;
  XftTextExtents8(fl_display, current_font, (XftChar8 *)str, n, &i);
  return i.xOff;
}

static void
xft_font_draw(const char *str, int n, int x, int y) {
  static XftDraw *draw = 0;
  if (!draw)
    draw = XftDrawCreate(fl_display, fl_window, fl_visual->visual, fl_colormap);
  else
    XftDrawChange(draw, fl_window);
  Region region = fl_region();
  if (region) {
    if (XEmptyRegion(region)) return;
    XftDrawSetClip(draw, region);
  }

  // Use fltk's color allocator, copy the results to match what
  // XftCollorAllocValue returns:
  XftColor color;
  color.pixel = fl_pixel;
  uchar r,g,b; fl_get_color(fl_color_, r,g,b);
  color.color.red   = r*0x101;
  color.color.green = g*0x101;
  color.color.blue  = b*0x101;
  color.color.alpha = 0xffff;

  XftDrawString8(draw, &color, current_font, x+fl_x_, y+fl_y_,
                    (XftChar8 *)str, n);
}

extern "C" {
static int sort_function(const void *aa, const void *bb) {
  const char* name_a = (*(Fl_Font_**)aa)->name_;
  const char* name_b = (*(Fl_Font_**)bb)->name_;
  int ret = strcasecmp(name_a+1, name_b+1); if (ret) return ret;
  return name_a[0]-name_b[0]; // sort by attribute
}
}

static Fl_Font_* make_a_font(char attrib, const char* name) {
  Fl_Font_* newfont = new Fl_Font_;
  char *n = new char[strlen(name)+2];
  n[0] = attrib;
  strcpy(n+1, name);
  newfont->name_ = n;
  newfont->bold_ = newfont;
  newfont->italic_ = newfont;
  newfont->first = 0;
  return newfont;
}

int
xft_font_list(Fl_Font*& arrayp) {
  static Fl_Font *font_array = 0;
  static int num_fonts = 0;

  if (font_array) { arrayp = font_array; return num_fonts; }

  XftFontSet *fs;
  char *name;
  fl_open_display();
  fs = XftListFonts(fl_display, fl_screen, 0, XFT_FAMILY, 0);
  num_fonts = fs->nfont;
  font_array = (Fl_Font *)calloc(num_fonts, sizeof(Fl_Font *));
  for (int i = 0; i < num_fonts; i++) {
    if (XftPatternGetString(fs->fonts[i], XFT_FAMILY, 0, &name) == XftResultMatch) {
      Fl_Font_* base = make_a_font(' ', name);
      base->italic_ = make_a_font('I', name);
      //if (bNeedBold) {
	base->bold_ = make_a_font('B', name);
	base->italic_->bold_ = base->bold_->italic_ = make_a_font('P', name);
	//}
      font_array[i] = base;
    }
  }
  XftFontSetDestroy(fs);

  qsort(font_array, num_fonts, sizeof(Fl_Font), sort_function);

  arrayp = font_array;
  return num_fonts;
}

extern "C" {
static int int_sort(const void *aa, const void *bb) {
  return (*(int*)aa)-(*(int*)bb);
}
}

int Fl_Font_::sizes(int*& sizep) const {
  fl_open_display();
  XftFontSet* fs = XftListFonts(fl_display, fl_screen,
				XFT_FAMILY, XftTypeString, name_+1, 0,
				XFT_PIXEL_SIZE, 0);
  static int* array = 0;
  static int array_size = 0;
  if (fs->nfont >= array_size) {
    delete[] array;
    array = new int[array_size = fs->nfont+1];
    array[0] = 0; // claim all fonts can scale
  }
  int j = 1;
  for (int i = 0; i < fs->nfont; i++) {
    double v;
    if (XftPatternGetDouble(fs->fonts[i], XFT_PIXEL_SIZE, 0, &v) == XftResultMatch) {
      array[j++] = int(v);
    }
  }
  qsort(array+1, j-1, sizeof(int), int_sort);
  XftFontSetDestroy(fs);
  sizep = array;
  return j;
}

int Fl_Font_::encodings(const char**& arrayp) const {
  fl_open_display();
  // we need to keep the previous return value around so the strings are
  // not destroyed.
  static XftFontSet* fs;
  if (fs) XftFontSetDestroy(fs);
  fs = XftListFonts(fl_display, fl_screen,
		    XFT_FAMILY, XftTypeString, name_+1, 0,
		    XFT_ENCODING, 0);
  static const char** array = 0;
  static int array_size = 0;
  if (fs->nfont > array_size) {
    delete[] array;
    array = new (const char*)[array_size = fs->nfont];
  }
  int j = 0;
  for (int i = 0; i < fs->nfont; i++) {
    char* v;
    if (XftPatternGetString(fs->fonts[i], XFT_ENCODING, 0, &v) == XftResultMatch) {
      array[j++] = v;
    }
  }
  arrayp = array;
  return j;
}

int
fl_list_fonts(Fl_Font*& arrayp) {
  return fl_font_renderer->list(arrayp);
}

// The predefined fonts that fltk has:  bold:       italic:
Fl_Font_
fl_fonts[] = {
{" sans",		fl_fonts+1, fl_fonts+2},
{"Bsans",		fl_fonts+1, fl_fonts+3},
{"Isans",		fl_fonts+3, fl_fonts+2},
{"Psans",		fl_fonts+3, fl_fonts+3},
{" mono",		fl_fonts+5, fl_fonts+6},
{"Bmono",		fl_fonts+5, fl_fonts+7},
{"Imono",		fl_fonts+7, fl_fonts+6},
{"Pmono",		fl_fonts+7, fl_fonts+7},
{" serif",		fl_fonts+9, fl_fonts+10},
{"Bserif",		fl_fonts+9, fl_fonts+11},
{"Iserif",		fl_fonts+11,fl_fonts+10},
{"Pserif",		fl_fonts+11,fl_fonts+11},
{" symbol",		fl_fonts+12,fl_fonts+12},
{" screen",		fl_fonts+14,fl_fonts+14},
{"Bscreen",		fl_fonts+14,fl_fonts+14},
{" dingbats",		fl_fonts+15,fl_fonts+15},
};

// turn a stored font name into a pretty name:
const char* Fl_Font_::name(int* ap) const {
  int type;
  switch (name_[0]) {
  case 'B': type = FL_BOLD; break;
  case 'I': type = FL_ITALIC; break;
  case 'P': type = FL_BOLD | FL_ITALIC; break;
  default:  type = 0; break;
  }
  if (ap) {*ap = type; return name_+1;}
  if (!type) {return name_+1;}
  static char *buffer = new char[128];
  strcpy(buffer, name_+1);
  if (type & FL_BOLD) strcat(buffer, " bold");
  if (type & FL_ITALIC) strcat(buffer, " italic");
  return buffer;
}

static Fl_Font_Renderer
xft_renderer = {
  xft_font, xft_font_load, xft_font_unload, xft_font_height, xft_font_descent,
  xft_font_width, xft_font_draw, xft_font_clip, xft_font_list, fl_fonts
};

#if 0
int
fl_xft() {

  fl_open_display();
  if (!XftDefaultHasRender(fl_display)) {
    fprintf(stderr, "Cannot start Xft plugin: Display does not "
                    "have render capability.\n");
    return 3;
  }
  fl_font_renderer = &xft_renderer;
  char temp[4];
  if (!fl_getconf("xft plugin/minimum antialias size", temp, sizeof(temp)))
    min_antialias_size = atol(temp);

  return 0;
}
#endif


//
// End of "$Id: fl_xft.cxx,v 1.4 2001/11/08 08:13:49 spitzak Exp $"
//
