// This file is distributed under the terms of the GNU General Public license.
// Copyright (C) 2000 Laurel Fan (See the file COPYING for details).

#ifndef SHELL_H
#define SHELL_H

#include "Command.h"
#include "Terminal.h"
#include "Descriptor.h"

#include <vector>
#include <stdiostream.h>

extern "C"{
  #include <readline/readline.h>
}

/*
  Shell that reads from stdin, writes to stdout.
  It's a Singleton because there's only one stdin and stdout,
  and because readline is global all over anyway

  The inheritance from Descriptor replaces what was previously
  TermInput.
*/

class TermShell : public Command::Shell, public Descriptor{
public:
  static TermShell *Instance();
  int setGatherer(Command::Gatherer *g);

  //: Callback for output handling
  int output(string s);
  //: Sets up the ansi/vt100 screen
  int setup_screen(int rows, int cols, int prompt_rows, string title);

  int run(string s);

  //: Handler (from Descriptor)
  void Handler(SelectSet *);  
  //: readline callback..
  void rl_callback();
private:
  TermShell();
  static TermShell *instanceP;

  vector<string> parse(string s);
  Command::Gatherer * m_gatherer;
  ostream *m_outs;

  int m_rows, m_cols, m_prompt_rows;
  string m_title;
};

#endif /* SHELL_H */
