// This file is distributed under the terms of the GNU General Public license.
// Copyright (C) 2000 Laurel Fan (See the file COPYING for details).

#include "Shell.h"

TermShell* TermShell::instanceP = 0;

TermShell *TermShell::Instance()
{
  if(!instanceP){
    instanceP = new TermShell;
  }
  return instanceP;
}

TermShell::TermShell()
{
  m_outs = &cout;
  m_gatherer = NULL;
  aconn = NULL;
  this->set_fd(0, FD_INP);
}

int TermShell::setGatherer(Command::Gatherer *g)
{
  assert(g);
  m_gatherer = g;
  assert(m_gatherer);
  m_outs = &cout;
}

int Command::Shell::run(string s)
{
  vector<string> args;

  output("test " + s);
  
   args = parse(s);

   cerr << "[" << args[0] << "]\n";
   assert(m_gatherer);
   m_gatherer->run("", args, s);
}

int TermShell::run(string s)
{
  vector<string> args;

  output("test " + s);
  
   args = parse(s);

   cerr << "[" << args[0] << "]\n";
   assert(m_gatherer);
   output(m_gatherer->run("", args, s));
}

int TermShell::output(string s)
{
  Terminal::save_cursor_pos(*m_outs);
  Terminal::set_cursor_pos(*m_outs, m_rows-m_prompt_rows-1, 0);
  *m_outs << s << "\n";
  Terminal::restore_cursor_pos(*m_outs);
}

vector<string> TermShell::parse(string s)
{
  vector<string> ret;
  string ws = " \r\n\t";
  int start_word, start_ws;
  
  // simply splits on whitespace.  no metacharacters, no quoting, no nothing
  start_word = 0;
  start_ws = -1;
  while(start_word != -1){
    start_ws = s.find_first_of(ws, start_word);
    ret.push_back(string(s, start_word, start_ws-start_word));
    start_word = s.find_first_not_of(ws, start_ws);
  }
  return(ret);
}

void TermShell::Handler(SelectSet *)
{
  rl_callback_read_char();
}

void echofun()
{
  TermShell::Instance()->rl_callback();
}

void TermShell::rl_callback(){
  //  Command::Shell::run(rl_line_buffer);
  TermShell::run(rl_line_buffer);
}

int TermShell::setup_screen(int rows, int cols, int prompt_rows, string title)
{
  // Screen Layout:
  // 1 row title                     (row 0)
  // m_rows-m_prompt_rows-2 output   (rows 1 to m_rows-m_prompt_rows-1) 
  // 1 row status                    (row m_rows-m_prompt_rows)
  // m_prompt_rows input             (row m_rows)

  m_rows = rows;
  m_cols = cols;
  m_prompt_rows = prompt_rows;
  m_title = title;

  if(m_rows){
    Terminal::Init(*m_outs, Terminal::ANSI);
  }else{
    Terminal::Init(*m_outs, Terminal::DUMB);
  }


  // draw the title at the top.
  Terminal::set_cursor_pos(*m_outs, 0, 0);
  *m_outs << foreground(Terminal::BLACK) << background(Terminal::GREEN)
	  << attribute(Terminal::BOLD)
	  << m_title << attribute(Terminal::NORMAL);
  
  // draw the status line.
  Terminal::set_cursor_pos(*m_outs, m_rows-m_prompt_rows, 0);
  *m_outs << foreground(Terminal::YELLOW) << background(Terminal::RED);
    for(int i=0;i<m_cols;i++){
      *m_outs << '-';
    }
    *m_outs << attribute(Terminal::NORMAL);
    
    // set up scroll region
    Terminal::scroll_region(*m_outs, 2, m_rows-m_prompt_rows-1);
    Terminal::set_cursor_pos(*m_outs, m_rows-m_prompt_rows-1, 0);
    
    // move to the prompt line.
    Terminal::set_cursor_pos(*m_outs, m_rows, 0);
    
    rl_callback_handler_install("", echofun);
}
#if COMMENTED_OUT
int main()
{
  TermShell *ts;

  SelectSet * dset = new SelectSet;

  ts = TermShell::Instance();

  ts->setup_screen(24, 80, 1, "Clay TermShell test");
  
  dset->Register(ts);
  
  while(1){
    dset->Select();
  }
}
#endif /* COMMENTED OUT */
