// This file is distributed under the terms of the GNU General Public license.
// Copyright (C) 2000 Alistair Riddoch (See the file COPYING for details).

#ifndef DESCRIPTOR_H
#define DESCRIPTOR_H

#include <list>

#define FD_INP 1<<0
#define FD_OUT 1<<1
#define FD_EXP 1<<2

// #include <Atlas/Net/TCPSocket.h>

class Descriptor;

class SelectSet {
#define DS_SIZE 
    list<Descriptor *> fds;

    public:
        void Register(Descriptor * new_descriptor) {
            fds.push_back(new_descriptor);
        }
        void UnRegister(Descriptor * old_descriptor) {
            /* fds.remove(old_descriptor); */
        }
 

        void Select();
};

class Descriptor
{
    int fd;
    int event_mask;
    public:
	virtual void Handler(SelectSet *) = 0;

        void set_fd(int rfd, int mask) {
            fd = rfd;
            event_mask = mask;
        }
        int get_fd() {
            return fd;
        }
};

class SocketDescriptor: public Descriptor {
    protected:
        //Atlas::Socket * asock;
};
#endif /* DESCRIPTOR_H */
