// This file is distributed under the terms of the GNU General Public license.
// Copyright (C) 2000 Alistair Riddoch (See the file COPYING for details).

#ifndef CLAY_NET_H
#define CLAY_NET_H

#include <list>

//#include <Atlas/Net/TCPSocket.h>

#include "Descriptor.h"


// Open a welknown socket and accept tcp connections.
template<class ClientType> class TCPServer : public SocketDescriptor {
    public:
	void Handler(SelectSet *dset) {
            cerr << "server handler\n";
            //Atlas::Socket * newconn = asock->accept();
            ClientType* newclient = new ClientType(newconn);
            dset->Register(newclient);
        }

        TCPServer(int port) {
            cerr << "set up server\n";
            // asock = new Atlas::TCPSocket();
            asock->listen("0.0.0.0", port, 5);
            set_fd(asock->getSock(), FD_INP);
        }


};

#if 0
class TelnetClient : public SocketDescriptor {
    public:
        void Handler(SelectSet *);

        TelnetClient(Atlas::Socket *);
};

class HttpSession {
    public:
        string session_name;
        HttpSession(int);
        HttpSession(string);
};

#define HTTPCON_ERR                  1
#define HTTPCON_NEWCONN              2
#define HTTPCON_STATUS_OK            3
#define HTTPCON_SESSION_FOUND        4
#define HTTPCON_HEADER_DONE          5


class HttpClient : public SocketDescriptor {
        static list<HttpSession *> sessions;
        static int session_count/* = 0*/;
        HttpSession * session;
        int status;
        void req_status(int);
        void intro_page();
        int find_session(string);
    public:
        void Handler(SelectSet *);

        HttpClient(Atlas::Socket *);

        static void AddSession();
        static void AddSession(string);
};

#define IRCCON_ERR                   1
#define IRCCON_NICK                  2
#define IRCCON_USER                  3
#define IRCCON_JOIN                  4

class IrcClient : public SocketDescriptor {
    public:
        void Handler(SelectSet *);

        IrcClient(Atlas::Socket *);
};

#endif

#endif /* CLAY_NET_H */
