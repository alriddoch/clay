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
    #include <sys/time.h>
    #include <sys/types.h>
    #include <unistd.h>
    #include <fcntl.h>
    #include <readline/readline.h>
}

#include "Net.h"
#include "Net_atlas.h"
#include "Shell.h"

void SelectSet::Select()
{
    fd_set rfds;
    list<Descriptor *>::iterator fdi;
    int tfd, mfd = -1;
    struct timeval tv;

    FD_ZERO(&rfds);
    cerr << "(";
    for(fdi = fds.begin(); fdi != fds.end(); fdi++) {
        tfd = (*fdi)->get_fd();
        if (tfd == -1) {
            cerr << "Duff one\n";
            UnRegister(*fdi);
        } else {
            cerr << " " << tfd << " ";
            FD_SET(tfd, &rfds);
            if (tfd > mfd) {
                mfd = tfd;
            }
        }
    }
    cerr << ")\n";

    tv.tv_sec = 5;
    tv.tv_usec = 0;

    cerr << "selecting\n";
    if (!select(mfd + 1, &rfds, NULL, NULL, &tv)) {
        cerr << "Nothing\n";
        return;
    }

    for(fdi = fds.begin(); fdi != fds.end(); fdi++) {
        tfd = (*fdi)->get_fd();
        if (tfd != -1 && FD_ISSET(tfd, &rfds)) {
            cerr << "Handling\n";
            (*fdi)->Handler(this);
        }
    }

}
// Test main function for setting up the various types of server socket
// possible
//

#if COMMENTED_OUT
int main(int argc, char ** argv)
{
#define TEST_ADDR "127.0.0.1"
  //#define TEST_ADDR "www.worldforge.net"

    SelectSet * dset = new SelectSet;
    Descriptor * sport = new TCPServer<TelnetClient>(6770);
    Descriptor * hport = new TCPServer<HttpClient>(6780);
    Descriptor * iport = new TCPServer<IrcClient>(6667);
    TermShell * in = TermShell::Instance();
    in->setup_screen(52, 80, 1, "Clay Connection test");
    AtlasConnection * aconn = new AtlasConnection(TEST_ADDR, in);

    dset->Register(in);
    dset->Register(sport);
    dset->Register(hport);
    dset->Register(iport);
    dset->Register(aconn);

    while(1){
        dset->Select();
    }

}
#endif


