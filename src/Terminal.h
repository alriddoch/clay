// This file is distributed under the terms of the GNU General Public license.
// Copyright (C) 2000 Michael Day (See the file COPYING for details).

#ifndef TERMINAL_H
#define TERMINAL_H

#include <iomanip>
/* #include "Command.h" */

namespace Terminal
{
    enum Type
    {
	DUMB,
	ANSI,
	HTML,
    };

    enum Attribute
    {
	NORMAL,
	BOLD,
	UNDERLINE,
	BLINK,
	REVERSE,
    };

    enum Color
    {
	BLACK,
	RED,
	GREEN,
	YELLOW,
	BLUE,
	MAGENTA,
	CYAN,
	WHITE,
    };

    void Init(ostream&, Type);

    ostream& normal(ostream&);
    ostream& bold(ostream&);
    ostream& underline(ostream&);
    ostream& blink(ostream&);
    ostream& reverse(ostream&);

    ostream& black(ostream&);
    ostream& red(ostream&);
    ostream& green(ostream&);
    ostream& yellow(ostream&);
    ostream& blue(ostream&);
    ostream& magenta(ostream&);
    ostream& cyan(ostream&);
    ostream& white(ostream&);

    ostream& set_attribute(ostream&, Attribute);
    ostream& set_foreground(ostream&, Color);
    ostream& set_background(ostream&, Color);

    ostream& save_cursor_pos(ostream &outs);
    ostream& restore_cursor_pos(ostream &outs);
    ostream& set_cursor_pos(ostream&, int row, int col);
    ostream& clear_screen(ostream&);
    ostream& scroll_region(ostream& outs, int top_row, int bottom_row);
    
    inline omanip<Attribute> attribute(Attribute a)
    {
	return omanip<Attribute>(set_attribute, a);
    }
    
    inline omanip<Color> foreground(Color c)
    {
	return omanip<Color>(set_foreground, c);
    }
    
    inline omanip<Color> background(Color c)
    {
	return omanip<Color>(set_background, c);
    }
}

#endif
