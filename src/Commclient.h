// This file is distributed under the terms of the GNU General Public license.
// Copyright (C) 2000 Alistair Riddoch (See the file COPYING for details).

#ifndef CLAY_COMMCLIENT_H
#define CLAY_COMMCLIENT_H

#include <Atlas/Message/Object.h>
#include <Atlas/Net/Stream.h>
#include <Atlas/Objects/Root.h>
#include <Atlas/Objects/Encoder.h>
#include <Atlas/Objects/Decoder.h>
#include <Atlas/Objects/Operation/RootOperation.h>
#include <Atlas/Objects/Operation/Login.h>
#include <Atlas/Objects/Operation/Create.h>
#include <Atlas/Objects/Operation/Look.h>
#include <Atlas/Objects/Operation/Sound.h>
#include <Atlas/Objects/Operation/Info.h>
#include <Atlas/Objects/Operation/Error.h>
#include <Atlas/Objects/Operation/Sight.h>

#include <list>
#include <string>

#include "Command.h"

class sockbuf : public filebuf {
  public:
    sockbuf() { }
    sockbuf(int fd) : filebuf(fd) { }
    virtual streampos sys_seek(streamoff, _seek_dir) { return streampos(); }
};

using namespace Atlas;
using Message::Object;

class ClayClient: public Objects::Decoder {
  private:
    int client_fd;
    int reply_flag;
    sockbuf client_buf;
    iostream client_ios;
    list<string> idLookList;
    Command::Shell* c_shell;
    Objects::Encoder * encoder;
    Codec<iostream> * codec;

  public:
    ClayClient(int fd, Command::Shell* shell) :
        client_fd(fd), client_buf(fd), client_ios(&client_buf),
        c_shell(shell), reply(NULL), reply_flag(0) { }

    void send(Objects::Root * op) {
        encoder->StreamMessage(op);
    }

    void Poll() {
        codec->Poll();
    }

    int send_wait(Objects::Operation::RootOperation * op,
                  Objects::Operation::RootOperation ** obj);
    int setup();

    void ObjectArrived(const Objects::Operation::Info&);
    void ObjectArrived(const Objects::Operation::Error&);
    void ObjectArrived(const Objects::Operation::Sight&);

    Object c_accountInfo;
    Object c_character;

    int snum;
    string c_accountId, c_characterId;
    Objects::Operation::RootOperation * reply;
};

#endif /* CLAY_COMMCLIENT_H */
