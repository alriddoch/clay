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

#include "Commclient.h"

int ClayClient::setup()
{
    Net::StreamConnect conn("clay", client_ios, this);

    c_shell->output("Negotiating... ");
    // conn.Poll() does all the negotiation
    while (conn.GetState() == Negotiate<iostream>::IN_PROGRESS) {
        conn.Poll();
    }
    c_shell->output("done");

    // Check whether negotiation was successful
    if (conn.GetState() == Negotiate<iostream>::FAILED) {
        c_shell->output("Failed to negotiate");
        return(-1);
    }
    // Negotiation was successful

    // Get the codec that negotiation established
    codec = conn.GetCodec();

    // This should always be sent at the beginning of a session

    encoder = new Objects::Encoder(codec);

    codec->StreamBegin();
    return(0);
}

int ClayClient::send_wait(Objects::Operation::RootOperation * obj,
                           Objects::Operation::RootOperation  ** ret)
{
    // Need to modify this to use serial numbers and reference numbers
    send(obj);
    reply = NULL;
    reply_flag=0;
    while (!reply_flag) {
        codec->Poll();
    }
    if (reply == NULL) {
        return(-1);
    }
    *ret = reply;
    try {
        string & ret_type = reply->GetParents().front().AsString();
        if (ret_type == "error") {
            cout << "GOT an ERROR op" << endl << flush;
            return(1);
        } else {
            cout << "Not an error op" << endl << flush;
            return(0);
        }
    }
    catch (Message::WrongTypeException) {
        cout << "EXCEPTION: COuld not get type of op" << endl << flush;
        return(1);
    }
}

void ClayClient::ObjectArrived(const Objects::Operation::Info & op)
{
   reply_flag = 1;
   reply = new Objects::Operation::Info(op);
}

void ClayClient::ObjectArrived(const Objects::Operation::Error & op)
{
   reply_flag = 1;
   reply = new Objects::Operation::Error(op);
}

void ClayClient::ObjectArrived(const Objects::Operation::Sight & op)
{
   reply_flag = 1;
   reply = new Objects::Operation::Sight(op);
   c_shell->output("Got sight op\n");
}
