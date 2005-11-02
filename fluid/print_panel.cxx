//
// "$Id$"
//
// FLUID print panel for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2005 by Bill Spitzak and others.
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
// Please report all bugs and problems on the following page:
//
//     http://www.fltk.org/str.php
//

// generated by Fast Light User Interface Designer (fluid) version 1.0107

#include "print_panel.h"
#include <stdio.h>
#include <stdlib.h>
#include "../src/flstring.h"
#include <FL/Fl_Preferences.H>
extern Fl_Preferences fluid_prefs;

Fl_Double_Window *print_panel=(Fl_Double_Window *)0;

Fl_Group *print_panel_controls=(Fl_Group *)0;

Fl_Choice *print_choice=(Fl_Choice *)0;

static void cb_print_choice(Fl_Choice*, void*) {
  print_update_status();
}

Fl_Button *print_properties=(Fl_Button *)0;

static void cb_print_properties(Fl_Button*, void*) {
  print_properties_panel->show();
}

Fl_Box *print_status=(Fl_Box *)0;

Fl_Round_Button *print_all=(Fl_Round_Button *)0;

static void cb_print_all(Fl_Round_Button*, void*) {
  print_from->deactivate();
print_to->deactivate();
}

Fl_Round_Button *print_pages=(Fl_Round_Button *)0;

static void cb_print_pages(Fl_Round_Button*, void*) {
  print_from->activate();
print_to->activate();
}

Fl_Round_Button *print_selection=(Fl_Round_Button *)0;

static void cb_print_selection(Fl_Round_Button*, void*) {
  print_from->deactivate();
print_to->deactivate();
}

Fl_Input *print_from=(Fl_Input *)0;

Fl_Input *print_to=(Fl_Input *)0;

Fl_Spinner *print_copies=(Fl_Spinner *)0;

static void cb_print_copies(Fl_Spinner*, void*) {
  if (print_copies->value() == 1) {
  print_collate_button->deactivate();
  print_collate_group[0]->deactivate();
  print_collate_group[1]->deactivate();
} else {
  print_collate_button->activate();
  print_collate_group[0]->activate();
  print_collate_group[1]->activate();
};
}

Fl_Check_Button *print_collate_button=(Fl_Check_Button *)0;

static void cb_print_collate_button(Fl_Check_Button*, void*) {
  int i = print_collate_button->value() != 0;
print_collate_group[i]->show();
print_collate_group[1 - i]->hide();
}

Fl_Group *print_collate_group[2]={(Fl_Group *)0};

static void cb_Cancel(Fl_Button*, void*) {
  print_panel->hide();
}

Fl_Progress *print_progress=(Fl_Progress *)0;

Fl_Double_Window *print_properties_panel=(Fl_Double_Window *)0;

static void cb_print_properties_panel(Fl_Double_Window*, void*) {
  print_properties_panel->hide();
print_update_status();
}

Fl_Choice *print_page_size=(Fl_Choice *)0;

Fl_Menu_Item menu_print_page_size[] = {
 {"Letter", 0,  0, 0, 0, FL_NORMAL_LABEL, 0, 14, 0},
 {"A4", 0,  0, 0, 0, FL_NORMAL_LABEL, 0, 14, 0},
 {0,0,0,0,0,0,0,0,0}
};

#include <FL/Fl_Pixmap.H>
static const char *idata_print_color[] = {
"24 24 17 1",
" \tc None",
".\tc #FFFF00",
"+\tc #C8FF00",
"@\tc #00FF00",
"#\tc #FFC800",
"$\tc #FF0000",
"%\tc #00FFFF",
"&\tc #000000",
"*\tc #FF00FF",
"=\tc #00FFC8",
"-\tc #FF00C8",
";\tc #00C800",
">\tc #C80000",
",\tc #0000C8",
"\'\tc #0000FF",
")\tc #00C8FF",
"!\tc #C800FF",
"         ......         ",
"       ..........       ",
"      ............      ",
"     ..............     ",
"     ..............     ",
"    ................    ",
"    ................    ",
"    ................    ",
"    +@@@@@@+#$$$$$$#    ",
"   %@@@@@@@&&$$$$$$$*   ",
"  %%@@@@@@&&&&$$$$$$**  ",
" %%%=@@@@&&&&&&$$$$-*** ",
" %%%%@@@;&&&&&&>$$$**** ",
"%%%%%%@@&&&&&&&&$$******",
"%%%%%%%@&&&&&&&&$*******",
"%%%%%%%%,&&&&&&,********",
"%%%%%%%%\'\'\'\'\'\'\'\'********",
"%%%%%%%%\'\'\'\'\'\'\'\'********",
"%%%%%%%%\'\'\'\'\'\'\'\'********",
" %%%%%%%)\'\'\'\'\'\'!******* ",
" %%%%%%%%\'\'\'\'\'\'******** ",
"  %%%%%%%%\'\'\'\'********  ",
"   %%%%%%%%\'\'********   ",
"     %%%%%%  ******     "
};
static Fl_Pixmap image_print_color(idata_print_color);

static const char *idata_print_gray[] = {
"24 24 17 1",
" \tc None",
".\tc #E3E3E3",
"+\tc #D2D2D2",
"@\tc #969696",
"#\tc #C2C2C2",
"$\tc #4C4C4C",
"%\tc #B2B2B2",
"&\tc #000000",
"*\tc #696969",
"=\tc #ACACAC",
"-\tc #626262",
";\tc #767676",
">\tc #3C3C3C",
",\tc #161616",
"\'\tc #1C1C1C",
")\tc #929292",
"!\tc #585858",
"         ......         ",
"       ..........       ",
"      ............      ",
"     ..............     ",
"     ..............     ",
"    ................    ",
"    ................    ",
"    ................    ",
"    +@@@@@@+#$$$$$$#    ",
"   %@@@@@@@&&$$$$$$$*   ",
"  %%@@@@@@&&&&$$$$$$**  ",
" %%%=@@@@&&&&&&$$$$-*** ",
" %%%%@@@;&&&&&&>$$$**** ",
"%%%%%%@@&&&&&&&&$$******",
"%%%%%%%@&&&&&&&&$*******",
"%%%%%%%%,&&&&&&,********",
"%%%%%%%%\'\'\'\'\'\'\'\'********",
"%%%%%%%%\'\'\'\'\'\'\'\'********",
"%%%%%%%%\'\'\'\'\'\'\'\'********",
" %%%%%%%)\'\'\'\'\'\'!******* ",
" %%%%%%%%\'\'\'\'\'\'******** ",
"  %%%%%%%%\'\'\'\'********  ",
"   %%%%%%%%\'\'********   ",
"     %%%%%%  ******     "
};
static Fl_Pixmap image_print_gray(idata_print_gray);

Fl_Button *print_output_mode[4]={(Fl_Button *)0};

static void cb_Save(Fl_Return_Button*, void*) {
  print_properties_panel->hide();

char name[1024];
int val;
const char *printer = (const char *)print_choice->menu()[print_choice->value()].user_data();

snprintf(name, sizeof(name), "%s/page_size", printer);
fluid_prefs.set(name, print_page_size->value());

snprintf(name, sizeof(name), "%s/output_mode", printer);
for (val = 0; val < 4; val ++) {
  if (print_output_mode[val]->value()) break;
}
fluid_prefs.set(name, val);
}

static void cb_Cancel1(Fl_Button*, void*) {
  print_properties_panel->hide();
print_update_status();
}

static void cb_Use(Fl_Button*, void*) {
  print_properties_panel->hide();
}

Fl_Double_Window* make_print_panel() {
  Fl_Double_Window* w;
  { Fl_Double_Window* o = print_panel = new Fl_Double_Window(465, 235, "Print");
    w = o;
    { Fl_Group* o = print_panel_controls = new Fl_Group(10, 10, 447, 216);
      { Fl_Choice* o = print_choice = new Fl_Choice(113, 10, 181, 25, "Printer:");
        o->down_box(FL_BORDER_BOX);
        o->labelfont(1);
        o->callback((Fl_Callback*)cb_print_choice);
        o->when(FL_WHEN_CHANGED);
      }
      { Fl_Button* o = print_properties = new Fl_Button(294, 10, 105, 25, "Properties...");
        o->callback((Fl_Callback*)cb_print_properties);
      }
      { Fl_Box* o = print_status = new Fl_Box(111, 41, 288, 17, "printer/job status");
        o->align(68|FL_ALIGN_INSIDE);
      }
      { Fl_Group* o = new Fl_Group(10, 86, 227, 105, "Print Range");
        o->box(FL_THIN_DOWN_BOX);
        o->labelfont(1);
        o->align(FL_ALIGN_TOP_LEFT);
        { Fl_Round_Button* o = print_all = new Fl_Round_Button(20, 96, 38, 25, "All");
          o->type(102);
          o->down_box(FL_ROUND_DOWN_BOX);
          o->value(1);
          o->callback((Fl_Callback*)cb_print_all);
        }
        { Fl_Round_Button* o = print_pages = new Fl_Round_Button(20, 126, 64, 25, "Pages");
          o->type(102);
          o->down_box(FL_ROUND_DOWN_BOX);
          o->callback((Fl_Callback*)cb_print_pages);
        }
        { Fl_Round_Button* o = print_selection = new Fl_Round_Button(20, 156, 82, 25, "Selection");
          o->type(102);
          o->down_box(FL_ROUND_DOWN_BOX);
          o->callback((Fl_Callback*)cb_print_selection);
        }
        { Fl_Input* o = print_from = new Fl_Input(136, 126, 28, 25, "From:");
          o->type(2);
          o->textfont(4);
          o->deactivate();
        }
        { Fl_Input* o = print_to = new Fl_Input(199, 126, 28, 25, "To:");
          o->type(2);
          o->textfont(4);
          o->deactivate();
        }
        o->end();
      }
      { Fl_Group* o = new Fl_Group(247, 86, 210, 105, "Copies");
        o->box(FL_THIN_DOWN_BOX);
        o->labelfont(1);
        o->align(FL_ALIGN_TOP_LEFT);
        { Fl_Spinner* o = print_copies = new Fl_Spinner(321, 96, 45, 25, "# Copies:");
          o->callback((Fl_Callback*)cb_print_copies);
          o->when(FL_WHEN_CHANGED);
        }
        { Fl_Check_Button* o = print_collate_button = new Fl_Check_Button(376, 96, 64, 25, "Collate");
          o->down_box(FL_DOWN_BOX);
          o->callback((Fl_Callback*)cb_print_collate_button);
          o->when(FL_WHEN_CHANGED);
          o->deactivate();
        }
        { Fl_Group* o = print_collate_group[0] = new Fl_Group(257, 131, 191, 50);
          o->deactivate();
          { Fl_Box* o = new Fl_Box(287, 141, 30, 40, "1");
            o->box(FL_BORDER_BOX);
            o->color(FL_BACKGROUND2_COLOR);
            o->labelsize(11);
            o->align(FL_ALIGN_BOTTOM_RIGHT|FL_ALIGN_INSIDE);
            o->deactivate();
          }
          { Fl_Box* o = new Fl_Box(272, 136, 30, 40, "1");
            o->box(FL_BORDER_BOX);
            o->color(FL_BACKGROUND2_COLOR);
            o->labelsize(11);
            o->align(FL_ALIGN_BOTTOM_RIGHT|FL_ALIGN_INSIDE);
            o->deactivate();
          }
          { Fl_Box* o = new Fl_Box(257, 131, 30, 40, "1");
            o->box(FL_BORDER_BOX);
            o->color(FL_BACKGROUND2_COLOR);
            o->labelsize(11);
            o->align(FL_ALIGN_BOTTOM_RIGHT|FL_ALIGN_INSIDE);
            o->deactivate();
          }
          { Fl_Box* o = new Fl_Box(352, 141, 30, 40, "2");
            o->box(FL_BORDER_BOX);
            o->color(FL_BACKGROUND2_COLOR);
            o->labelsize(11);
            o->align(FL_ALIGN_BOTTOM_RIGHT|FL_ALIGN_INSIDE);
            o->deactivate();
          }
          { Fl_Box* o = new Fl_Box(337, 136, 30, 40, "2");
            o->box(FL_BORDER_BOX);
            o->color(FL_BACKGROUND2_COLOR);
            o->labelsize(11);
            o->align(FL_ALIGN_BOTTOM_RIGHT|FL_ALIGN_INSIDE);
            o->deactivate();
          }
          { Fl_Box* o = new Fl_Box(322, 131, 30, 40, "2");
            o->box(FL_BORDER_BOX);
            o->color(FL_BACKGROUND2_COLOR);
            o->labelsize(11);
            o->align(FL_ALIGN_BOTTOM_RIGHT|FL_ALIGN_INSIDE);
            o->deactivate();
          }
          { Fl_Box* o = new Fl_Box(417, 141, 30, 40, "3");
            o->box(FL_BORDER_BOX);
            o->color(FL_BACKGROUND2_COLOR);
            o->labelsize(11);
            o->align(FL_ALIGN_BOTTOM_RIGHT|FL_ALIGN_INSIDE);
            o->deactivate();
          }
          { Fl_Box* o = new Fl_Box(402, 136, 30, 40, "3");
            o->box(FL_BORDER_BOX);
            o->color(FL_BACKGROUND2_COLOR);
            o->labelsize(11);
            o->align(FL_ALIGN_BOTTOM_RIGHT|FL_ALIGN_INSIDE);
            o->deactivate();
          }
          { Fl_Box* o = new Fl_Box(387, 131, 30, 40, "3");
            o->box(FL_BORDER_BOX);
            o->color(FL_BACKGROUND2_COLOR);
            o->labelsize(11);
            o->align(FL_ALIGN_BOTTOM_RIGHT|FL_ALIGN_INSIDE);
            o->deactivate();
          }
          o->end();
        }
        { Fl_Group* o = print_collate_group[1] = new Fl_Group(257, 131, 191, 50);
          o->hide();
          o->deactivate();
          { Fl_Box* o = new Fl_Box(287, 141, 30, 40, "3");
            o->box(FL_BORDER_BOX);
            o->color(FL_BACKGROUND2_COLOR);
            o->labelsize(11);
            o->align(FL_ALIGN_BOTTOM_RIGHT|FL_ALIGN_INSIDE);
          }
          { Fl_Box* o = new Fl_Box(272, 136, 30, 40, "2");
            o->box(FL_BORDER_BOX);
            o->color(FL_BACKGROUND2_COLOR);
            o->labelsize(11);
            o->align(FL_ALIGN_BOTTOM_RIGHT|FL_ALIGN_INSIDE);
          }
          { Fl_Box* o = new Fl_Box(257, 131, 30, 40, "1");
            o->box(FL_BORDER_BOX);
            o->color(FL_BACKGROUND2_COLOR);
            o->labelsize(11);
            o->align(FL_ALIGN_BOTTOM_RIGHT|FL_ALIGN_INSIDE);
          }
          { Fl_Box* o = new Fl_Box(352, 141, 30, 40, "3");
            o->box(FL_BORDER_BOX);
            o->color(FL_BACKGROUND2_COLOR);
            o->labelsize(11);
            o->align(FL_ALIGN_BOTTOM_RIGHT|FL_ALIGN_INSIDE);
          }
          { Fl_Box* o = new Fl_Box(337, 136, 30, 40, "2");
            o->box(FL_BORDER_BOX);
            o->color(FL_BACKGROUND2_COLOR);
            o->labelsize(11);
            o->align(FL_ALIGN_BOTTOM_RIGHT|FL_ALIGN_INSIDE);
          }
          { Fl_Box* o = new Fl_Box(322, 131, 30, 40, "1");
            o->box(FL_BORDER_BOX);
            o->color(FL_BACKGROUND2_COLOR);
            o->labelsize(11);
            o->align(FL_ALIGN_BOTTOM_RIGHT|FL_ALIGN_INSIDE);
          }
          { Fl_Box* o = new Fl_Box(417, 141, 30, 40, "3");
            o->box(FL_BORDER_BOX);
            o->color(FL_BACKGROUND2_COLOR);
            o->labelsize(11);
            o->align(FL_ALIGN_BOTTOM_RIGHT|FL_ALIGN_INSIDE);
          }
          { Fl_Box* o = new Fl_Box(402, 136, 30, 40, "2");
            o->box(FL_BORDER_BOX);
            o->color(FL_BACKGROUND2_COLOR);
            o->labelsize(11);
            o->align(FL_ALIGN_BOTTOM_RIGHT|FL_ALIGN_INSIDE);
          }
          { Fl_Box* o = new Fl_Box(387, 131, 30, 40, "1");
            o->box(FL_BORDER_BOX);
            o->color(FL_BACKGROUND2_COLOR);
            o->labelsize(11);
            o->align(FL_ALIGN_BOTTOM_RIGHT|FL_ALIGN_INSIDE);
          }
          o->end();
        }
        o->end();
      }
      { Fl_Return_Button* o = new Fl_Return_Button(309, 201, 70, 25, "Print");
        o->callback((Fl_Callback*)print_cb);
      }
      { Fl_Button* o = new Fl_Button(389, 201, 68, 25, "Cancel");
        o->callback((Fl_Callback*)cb_Cancel);
      }
      o->end();
    }
    { Fl_Progress* o = print_progress = new Fl_Progress(10, 203, 289, 21);
      o->selection_color((Fl_Color)4);
      o->hide();
    }
    o->set_modal();
    o->end();
  }
  { Fl_Double_Window* o = print_properties_panel = new Fl_Double_Window(290, 130, "Printer Properties");
    w = o;
    o->callback((Fl_Callback*)cb_print_properties_panel);
    { Fl_Choice* o = print_page_size = new Fl_Choice(110, 10, 80, 25, "Page Size:");
      o->down_box(FL_BORDER_BOX);
      o->labelfont(1);
      o->menu(menu_print_page_size);
    }
    { Fl_Group* o = new Fl_Group(110, 45, 170, 40, "Output Mode:");
      o->labelfont(1);
      o->align(FL_ALIGN_LEFT);
      { Fl_Button* o = print_output_mode[0] = new Fl_Button(110, 45, 30, 40);
        o->type(102);
        o->box(FL_BORDER_BOX);
        o->down_box(FL_BORDER_BOX);
        o->value(1);
        o->color(FL_BACKGROUND2_COLOR);
        o->selection_color(FL_FOREGROUND_COLOR);
        o->image(image_print_color);
      }
      { Fl_Button* o = print_output_mode[1] = new Fl_Button(150, 50, 40, 30);
        o->type(102);
        o->box(FL_BORDER_BOX);
        o->down_box(FL_BORDER_BOX);
        o->color(FL_BACKGROUND2_COLOR);
        o->selection_color(FL_FOREGROUND_COLOR);
        o->image(image_print_color);
      }
      { Fl_Button* o = print_output_mode[2] = new Fl_Button(200, 45, 30, 40);
        o->type(102);
        o->box(FL_BORDER_BOX);
        o->down_box(FL_BORDER_BOX);
        o->color(FL_BACKGROUND2_COLOR);
        o->selection_color(FL_FOREGROUND_COLOR);
        o->image(image_print_gray);
      }
      { Fl_Button* o = print_output_mode[3] = new Fl_Button(240, 50, 40, 30);
        o->type(102);
        o->box(FL_BORDER_BOX);
        o->down_box(FL_BORDER_BOX);
        o->color(FL_BACKGROUND2_COLOR);
        o->selection_color(FL_FOREGROUND_COLOR);
        o->image(image_print_gray);
      }
      o->end();
    }
    { Fl_Return_Button* o = new Fl_Return_Button(123, 95, 79, 25, "Save");
      o->callback((Fl_Callback*)cb_Save);
    }
    { Fl_Button* o = new Fl_Button(212, 95, 68, 25, "Cancel");
      o->callback((Fl_Callback*)cb_Cancel1);
    }
    { Fl_Button* o = new Fl_Button(60, 95, 53, 25, "Use");
      o->callback((Fl_Callback*)cb_Use);
    }
    o->set_modal();
    o->end();
  }
  return w;
}
void print_cb(Fl_Return_Button *, void *);

void print_load() {
  FILE *lpstat;
char line[1024], name[1024], *nptr, qname[2048], *qptr, defname[1024];
int i;

if (print_choice->size() > 1) {
  for (i = 1; print_choice->text(i); i ++) {
    free(print_choice->menu()[i].user_data());
  }
}

print_choice->clear();
print_choice->add("Print To File", 0, 0, 0, FL_MENU_DIVIDER);
print_choice->value(0);

defname[0] = '\0';

if ((lpstat = popen("lpstat -p -d", "r")) != NULL) {
  while (fgets(line, sizeof(line), lpstat)) {
    if (!strncmp(line, "printer ", 8) &&
        sscanf(line + 8, "%s", name) == 1) {
      for (nptr = name, qptr = qname; *nptr; *qptr++ = *nptr++) {
        if (*nptr == '/') *qptr++ = '\\';
      }
      *qptr = '\0';

      print_choice->add(qname, 0, 0, (void *)strdup(name), 0);
    } else if (!strncmp(line, "system default destination: ", 28)) {
      if (sscanf(line + 28, "%s", defname) != 1) defname[0] = '\0';
    }
  }
  pclose(lpstat);
}

if (defname[0]) {
  for (i = 1; print_choice->text(i); i ++) {
    if (!strcmp((char *)print_choice->menu()[i].user_data(), defname)) {
      print_choice->value(i);
      break;
    }
  }
} else if (print_choice->size() > 2) print_choice->value(1);


print_update_status();
}

void print_update_status() {
  FILE *lpstat;
char command[1024];
static char status[1024];
const char *printer = (const char *)print_choice->menu()[print_choice->value()].user_data();

if (print_choice->value()) {
  snprintf(command, sizeof(command), "lpstat -p '%s'", printer);
  if ((lpstat = popen(command, "r")) != NULL) {
    fgets(status, sizeof(status), lpstat);
    pclose(lpstat);
  } else strcpy(status, "printer status unavailable");
} else status[0] = '\0';

print_status->label(status);

char name[1024];
int val;

snprintf(name, sizeof(name), "%s/page_size", printer);
fluid_prefs.get(name, val, 0);
print_page_size->value(val);

snprintf(name, sizeof(name), "%s/output_mode", printer);
fluid_prefs.get(name, val, 0);
print_output_mode[val]->setonly();
}

//
// End of "$Id$".
//
