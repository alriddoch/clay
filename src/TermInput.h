// This file is distributed under the terms of the GNU General Public license.
// Copyright (C) 2000 Alistair Riddoch (See the file COPYING for details).

#ifndef TERMINPUT_H
#define TERMINPUT_H

#include "Descriptor.h"

class TermInput : public Descriptor {
    public:
        TermInput(Atlas::Socket *);
        TermInput(int fd);
        TermInput();    

        void Handler(SelectSet *);
};

#endif /* TERMINPUT_H */
