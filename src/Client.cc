// This file is distributed under the terms of the GNU General Public license.
// Copyright (C) 2000 Michael Day (See the file COPYING for details).

#include "Client.h"

Client::Client(Atlas::Socket* sock, Atlas::Codec* codec)
 : Atlas::UserClient(sock, codec)
{
}
 
