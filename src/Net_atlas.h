// This file is distributed under the terms of the GNU General Public license.
// Copyright (C) 2000 Alistair Riddoch (See the file COPYING for details).

#ifndef CLAY_NET_ATLAS_H
#define CLAY_NET_ATLAS_H

#include <Atlas/Message/Object.h>
#include <Atlas/Objects/Root.h>
#include <Atlas/Objects/Operation/RootOperation.h>
#include <Atlas/Objects/Operation/Login.h>
#include <Atlas/Objects/Operation/Create.h>
#include <Atlas/Objects/Operation/Look.h>
#include <Atlas/Objects/Operation/Sound.h>

#include <list>
#include <string>

#include "Descriptor.h"
#include "Command.h"
#include "Commclient.h"

using namespace Atlas;
using Message::Object;

class AtlasConnection : public SocketDescriptor {
    ClayClient* c_client;
    Command::Shell* c_shell;
  public:
    AtlasConnection(Command::Shell * shell) : c_shell(shell) { }

    void send(Objects::Root * op) {
        c_client->send(op);
    }
    int send_wait(Objects::Operation::RootOperation * op,
                  Objects::Operation::RootOperation ** obj) {
        return c_client->send_wait(op, obj);
    }
    int setup(const string & host, int port);
    int login(string,string);
    int createCharacter(const string &);
    void lookWorld();
    void Handler(SelectSet *);
    void Say(const string & statement);
};

#endif /* CLAY_NET_ATLAS_H */
