// This file is distributed under the terms of the GNU General Public license.
// Copyright (C) 2000 Alistair Riddoch (See the file COPYING for details).

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <cassert>
#include <iostream>

extern "C" {
    #include <sys/time.h>
    #include <sys/types.h>
    #include <unistd.h>
    #include <fcntl.h>
}

#include "Net.h"

TelnetClient::TelnetClient(Atlas::Socket * newconn)
{
    cerr << "set up telnet client\n";
    int tmp_fd = newconn->getSock();
    asock = newconn;
    fcntl(tmp_fd, F_SETFL, O_NONBLOCK | fcntl(tmp_fd, F_GETFL));
    set_fd(tmp_fd, FD_INP);
}

void TelnetClient::Handler(SelectSet * dset)
{
    string inp;
    cerr << "client handler\n";
    if (asock->recv(inp) == 0) {
        // Close/Disconnect
        cerr << "BLUURG";
        asock->close();
        set_fd(asock->getSock(), FD_INP);
        dset->UnRegister(this);
    }
    cerr << "ANIT-";
    cerr << inp;
}
