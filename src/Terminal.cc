// This file is distributed under the terms of the GNU General Public license.
// Copyright (C) 2000 Michael Day (See the file COPYING for details).

#include "Terminal.h"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <map>
#include <string>
#include <cassert>

using std::map;
using std::string;

namespace Command
{
  class Shell;
}

namespace Terminal
{
    class OutputDriver
    {
	public:

      // set attributes (bold, underline, etc.) of output text
	virtual ostream& SetAttribute(ostream&, Attribute) = 0;
      // set foreground color of output text
	virtual ostream& SetForeground(ostream&, Color) = 0;
      // set background color of output text
        virtual ostream& SetBackground(ostream&, Color) = 0;

      // some vt100 stuff. just enough to implement.. split screen
      // set cursor position
        virtual ostream& SaveCursorPos(ostream&) = 0;
      // set cursor position
        virtual ostream& RestoreCursorPos(ostream&) = 0;
      // set cursor position
        virtual ostream& SetCursorPos(ostream&, int row, int col) = 0;
      // clear screen
        virtual ostream& ClearScreen(ostream&) = 0;
      // set a region to scroll
        virtual ostream& ScrollRegion(ostream&, int top_row, int bottom_row)
	  = 0;
    };
    class DumbDriver : public OutputDriver
    {
	public:

	virtual ostream& SetAttribute(ostream& outs, Attribute)
	{
	    return outs;
	}

	virtual ostream& SetForeground(ostream& outs, Color)
	{
	    return outs;
	}
	
	virtual ostream& SetBackground(ostream& outs, Color)
	{
	    return outs;
	}

        virtual ostream& RestoreCursorPos(ostream& outs)
	{
	    return outs;
	}

        virtual ostream& SaveCursorPos(ostream& outs)
	{
	    return outs;
	}

        virtual ostream& SetCursorPos(ostream& outs, int, int)
	{
	    return outs;
	}

        virtual ostream& ClearScreen(ostream& outs)
	{
	    outs << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
	    return outs;
	}

        virtual ostream& ScrollRegion(ostream& outs, int top_row,
				      int bottom_row)
	{
	  return outs;
	}
    };
    class AnsiDriver : public OutputDriver{
        private:
        string m_prompt;
      
	public:

	virtual ostream& SetAttribute(ostream& outs, Attribute attribute)
	{
	    switch (attribute)
	    {
		case NORMAL: outs << "\x1B[0m"; break;
		case BOLD: outs << "\x1B[1m"; break;
		case UNDERLINE: outs << "\x1B[4m"; break;
		case BLINK: outs << "\x1B[5m"; break;
		case REVERSE: outs << "\x1B[7m"; break;
	    }

	    return outs;
	}

	virtual ostream& SetForeground(ostream& outs, Color color)
	{
	    switch (color)
	    {
		case BLACK: outs << "\x1B[30m"; break;
		case RED: outs << "\x1B[31m"; break;
		case GREEN: outs << "\x1B[32m"; break;
		case YELLOW: outs << "\x1B[33m"; break;
		case BLUE: outs << "\x1B[34m"; break;
		case MAGENTA: outs << "\x1B[35m"; break;
		case CYAN: outs << "\x1B[36m"; break;
		case WHITE: outs << "\x1B[37m"; break;
	    }

	    return outs;
	}

	virtual ostream& SetBackground(ostream& outs, Color color)
	{
	    switch (color)
	    {
		case BLACK: outs << "\x1B[40m"; break;
		case RED: outs << "\x1B[41m"; break;
		case GREEN: outs << "\x1B[42m"; break;
		case YELLOW: outs << "\x1B[43m"; break;
		case BLUE: outs << "\x1B[44m"; break;
		case MAGENTA: outs << "\x1B[45m"; break;
		case CYAN: outs << "\x1B[46m"; break;
		case WHITE: outs << "\x1B[47m"; break;
	    }

	    return outs;
	}
        virtual ostream& SaveCursorPos(ostream &outs)
	{
	  outs << "\x1B" << "7" << flush;
	  return outs;
	}

        virtual ostream& RestoreCursorPos(ostream &outs)
	{
	  outs << "\x1B" << "8" << flush;
	  return outs;
	}

        virtual ostream& SetCursorPos(ostream& outs, int row, int col)
	{
	    outs << "\x1B[" << row << ";" << col << "f" << flush; 
	    return outs;
	}

        virtual ostream& ClearScreen(ostream& outs)
	{
  	    outs << "\x1B[2J" << flush;
	    return outs;
	}

        virtual ostream& ScrollRegion(ostream& outs,
				      int top_row, int bottom_row)
	{
	  outs << "\x1B[" << top_row << ";" << bottom_row << "r" << flush;
	}
    };
    namespace
    {
	map<ostream*, OutputDriver*> streams;

	OutputDriver *driver(ostream& outs)
	{
	    map<ostream*, OutputDriver*>::iterator i = streams.find(&outs);
	    assert(i != streams.end());
	    return i->second;
	}
    }
}

void Terminal::Init(ostream& outs, Type type)
{
    OutputDriver *driver;

    switch (type)
    {
	case DUMB:
	  driver = new DumbDriver;
	  break;
	case ANSI:
	  driver = new AnsiDriver;
	  break;
    }
    
    map<ostream*, OutputDriver*>::iterator i = streams.find(&outs);

    if (i == streams.end())
    {
	delete i->second;
    }

    streams[&outs] = driver;
    Terminal::clear_screen(outs);
}


ostream& Terminal::set_attribute(ostream& outs, Terminal::Attribute attribute)
{
    return driver(outs)->SetAttribute(outs, attribute);
}

ostream& Terminal::set_foreground(ostream& outs, Terminal::Color color)
{
    return driver(outs)->SetForeground(outs, color);
}

ostream& Terminal::set_background(ostream& outs, Terminal::Color color)
{
    return driver(outs)->SetBackground(outs, color);
}

ostream& Terminal::save_cursor_pos(ostream& outs)
{
    return driver(outs)->SaveCursorPos(outs);
}

ostream& Terminal::restore_cursor_pos(ostream& outs)
{
    return driver(outs)->RestoreCursorPos(outs);
}

ostream& Terminal::set_cursor_pos(ostream& outs, int row, int col)
{
    return driver(outs)->SetCursorPos(outs, row, col);
}

ostream& Terminal::clear_screen(ostream& outs)
{
    return driver(outs)->ClearScreen(outs);
}

ostream& Terminal::scroll_region(ostream& outs, int top_row, int bottom_row)
{
    return driver(outs)->ScrollRegion(outs, top_row, bottom_row);
}

ostream& Terminal::normal(ostream& outs)
{
    return outs << attribute(NORMAL);
}

ostream& Terminal::bold(ostream& outs)
{
    return outs << attribute(BOLD);
}

ostream& Terminal::underline(ostream& outs)
{
    return outs << attribute(UNDERLINE);
}

ostream& Terminal::blink(ostream& outs)
{
    return outs << attribute(BLINK);
}

ostream& Terminal::reverse(ostream& outs)
{
    return outs << attribute(REVERSE);
}

ostream& Terminal::black(ostream& outs)
{
    return (outs << foreground(BLACK));
}

ostream& Terminal::red(ostream& outs)
{
    return (outs << foreground(RED));
}

ostream& Terminal::green(ostream& outs)
{
    return (outs << foreground(GREEN));
}

ostream& Terminal::yellow(ostream& outs)
{
    return (outs << foreground(YELLOW));
}

ostream& Terminal::blue(ostream& outs)
{
    return (outs << foreground(BLUE));
}

ostream& Terminal::magenta(ostream& outs)
{
    return (outs << foreground(MAGENTA));
}

ostream& Terminal::cyan(ostream& outs)
{
    return (outs << foreground(CYAN));
}

ostream& Terminal::white(ostream& outs)
{
    return (outs << foreground(WHITE));
}
