// This file is distributed under the terms of the GNU General Public license.
// Copyright (C) 2000 Michael Day (See the file COPYING for details).

#ifndef CLIENT_H
#define CLIENT_H

#include <Atlas/Content/UserClient.h>

class Client : public Atlas::UserClient
{
    public:

    Client(Atlas::Socket* sock, Atlas::Codec* codec);
};

#endif
