// This file is distributed under the terms of the GNU General Public license.
// Copyright (C) 2000 Alistair Riddoch (See the file COPYING for details).

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <cassert>
#include <iostream>
#include <stdiostream.h>

using namespace std;

extern "C" {
    #include <readline/readline.h>
}

#include "TermInput.h"

void echofun();

TermInput::TermInput(int fd=0)
{
  this->set_fd(fd, FD_INP);
  //  rl_callback_handler_install("clay> ", *echofun);
}

void TermInput::Handler(SelectSet *)
{
  rl_callback_read_char();
}


