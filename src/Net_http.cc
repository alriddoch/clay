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

int HttpClient::session_count = 0;
list<HttpSession *> HttpClient::sessions;

HttpClient::HttpClient(Atlas::Socket * newconn)
{
    cerr << "new http connection\n";
    int tmp_fd = newconn->getSock();
    asock = newconn;
    fcntl(tmp_fd, F_SETFL, O_NONBLOCK | fcntl(tmp_fd, F_GETFL));
    set_fd(tmp_fd, FD_INP);
    status = HTTPCON_NEWCONN;
}

void HttpClient::intro_page()
{
    asock->send("<HTML><BODY>HELLO</BODY></HTML>\n");
}

void HttpClient::req_status(int client_status)
{
    switch (client_status) {
        case HTTPCON_SESSION_FOUND:
            cerr << "GOT sess\n";
            asock->send("HTTP/1.0 200 OK\r\n");
            asock->send("Server: Atlas::Clay (WorldForge proxy)\r\n");
            asock->send("Content-Type: text/html\r\n\r\n");
            intro_page();
            break;
        case HTTPCON_ERR:
            cerr << "GOT weird\n";
            asock->send("HTTP/1.0 301 Moved Permanently\r\n");
            asock->send("Location: /\r\n\r\n");
            break;
        case HTTPCON_STATUS_OK:
            cerr << "GOT new_sess\n";
            asock->send("HTTP/1.0 301 Moved Permanently\r\n");
            asock->send("Location: /session\r\n\r\n");
            break;
    }
}

int HttpClient::find_session(string sess_name)
{
    list<HttpSession *>::iterator sli;

    for(sli = sessions.begin(); sli != sessions.end(); sli++) {
        if ((*sli)->session_name.compare(sess_name) == 0) {
            return 1;
        }
    }
    return 0;
}


void HttpClient::Handler(SelectSet  * dset)
{
    string inp;
    string session_name;
    string::size_type loc=0;
    cerr << "http client handler\n";
    if (asock->recv(inp) == 0) {
        // Close/Disconnect
        asock->close();
        set_fd(asock->getSock(), FD_INP);
        dset->UnRegister(this);
    }
    while (1) {
        switch (status) {
            case HTTPCON_NEWCONN:
                if (inp.find("GET ",loc) == 0) {
                    cerr << "GOT GET\n";
                    inp.erase(0,4);
                    loc=inp.find(' ', 0);
                    session_name = inp.substr(0, loc);
                    inp.erase(0,loc);
                    if (session_name.compare("/")) {
                        session_name.erase(0,1);
                        if (find_session(session_name)) {
                            status = HTTPCON_SESSION_FOUND;
                        } else {
                            status = HTTPCON_ERR;
                        }
                    } else {
                        status = HTTPCON_STATUS_OK;
                    }
                } else return;
                break;
            case HTTPCON_SESSION_FOUND:
            case HTTPCON_ERR:
            case HTTPCON_STATUS_OK:
                req_status(status);
                status = HTTPCON_HEADER_DONE;
                asock->close();
                set_fd(asock->getSock(), FD_INP);
                /* Drop through */
            case HTTPCON_HEADER_DONE:
                return;
        }
    }
    cerr << inp;
}

void HttpClient::AddSession(string new_name)
{
    sessions.push_back(new HttpSession(new_name));
}

void HttpClient::AddSession()
{
    sessions.push_back(new HttpSession(session_count++));
}

HttpSession::HttpSession(int number)
{
    session_name = "session";
}

HttpSession::HttpSession(string name)
{
    session_name = name;
}
