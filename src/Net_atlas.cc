// This file is distributed under the terms of the GNU General Public license.
// Copyright (C) 2000 Alistair Riddoch (See the file COPYING for details).

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <cassert>
#include <iostream>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

extern "C" {
    #include <sys/time.h>
    #include <sys/types.h>
    #include <unistd.h>
    #include <fcntl.h>
}

#include "Net_atlas.h"

int AtlasConnection::setup(const string & host, int port)
{
    int res, tmp_fd;

    tmp_fd = socket(PF_INET, SOCK_STREAM, 0);
    if (tmp_fd < 0) {
        c_shell->output("Connection failed...\n");
        return(-1);
    }

    struct sockaddr_in sin;
    sin.sin_family = AF_INET;
    sin.sin_port = htons(port);
    sin.sin_addr.s_addr = htonl(0x7f000001);
  

    res = ::connect(tmp_fd, (struct sockaddr *)&sin, sizeof(sin));
    if (res == -1) {
        close(tmp_fd);
        c_shell->output("Connection failed...\n");
        return(-1);
    }

    c_client = new ClayClient(tmp_fd, c_shell);

    res = c_client->setup();
    if (res == -1) {
        delete c_client;
        close(tmp_fd);
        return(-1);
    }

    set_fd(tmp_fd, FD_INP);
    return 0;
}

int AtlasConnection::login(string account, string password)
{
    int result;
    Object::MapType player;
    player["id"] = account;
    player["password"] = password;
    player["parents"] = Object::ListType(1,Object("player"));
    
    //at first we try to just login...
    Objects::Operation::RootOperation * loginOp = new Objects::Operation::Login();
    *loginOp = Objects::Operation::Login::Instantiate();

    loginOp->SetArgs(Object::ListType(1,player));
    Objects::Operation::RootOperation * response;
    int ret = send_wait(loginOp, &response);
    if (ret == 0) {
        c_shell->output("Login ok\n");
        c_client->c_accountInfo = response->GetArgs().front();
        c_client->c_accountId = c_client->c_accountInfo.AsMap()["id"].AsString();
        return 0;
    }
    c_shell->output("Login failed: ");
        
    c_shell->output("Trying to create account..\n");
    //at first we try to just create...
    Objects::Operation::RootOperation * createOp = new Objects::Operation::Create();
    *createOp = Objects::Operation::Create::Instantiate();

    createOp->SetArgs(Object::ListType(1,player));

    ret = send_wait(createOp, &response);
    if (ret == 0) {
        c_shell->output("Created account ok\n");
        c_client->c_accountInfo = response->GetArgs().front();
        c_client->c_accountId = c_client->c_accountInfo.AsMap()["id"].AsString();
        return 0;
    }
        
    c_shell->output("Account creation failed: ");
    return -1;
}

int AtlasConnection::createCharacter(const string & name)
{
    int ret;
    //looking first at account info to see if we have character
    Object::MapType & account = c_client->c_accountInfo.AsMap();
    if ((account.find("characters") != account.end()) &&
        (account["characters"].AsList().size() != 0)) {
        Object::ListType characters = account["characters"].AsList();
        c_client->c_characterId = characters.front().AsString();
        Objects::Operation::RootOperation * lookOp = new Objects::Operation::Look();
        *lookOp = Objects::Operation::Look::Instantiate();
        lookOp->SetTo(c_client->c_characterId);
        lookOp->SetFrom(c_client->c_accountId);
        Objects::Operation::RootOperation * response;
        ret = send_wait(lookOp,&response);
        if (ret == 0) {
            c_client->c_character = response->GetArgs().front();
            return 0;
        }
        c_shell->output("Error fetching character\n");
    }

    c_shell->output("Try to create character...\n");
    //use something else if you don't want account==character name

    Object::MapType character;
    character["gender"] = Object("female");
    character["parents"] = Object::ListType(1,Object("farmer"));
    character["name"] = Object(name);

    Objects::Operation::RootOperation * createOp = new Objects::Operation::Create();
    *createOp = Objects::Operation::Create::Instantiate();
    createOp->SetFrom(c_client->c_accountId);

    createOp->SetArgs(Object::ListType(1,character));


    Objects::Operation::RootOperation * response;
    ret = send_wait(createOp,&response);
    if (ret == 0) {
        c_client->c_character = response->GetArgs().front();
        c_client->c_characterId = c_client->c_character.AsMap()["id"].AsString();
        return 0;
    }
    c_shell->output("Error creating character\n");
    
    return -1;
}

void AtlasConnection::Say(const string & statement)
{
    Objects::Operation::RootOperation * soundOp = new Objects::Operation::Talk();
    *soundOp = Objects::Operation::Sound::Instantiate();
    soundOp->SetFrom(c_client->c_characterId);
    Object::MapType speech;
    speech["say"] = Object(statement);
    soundOp->SetArgs(Object::ListType(1,Object(Object(speech))));
    send(soundOp);
}

void AtlasConnection::lookWorld()
{
    Objects::Operation::RootOperation * lookOp = new Objects::Operation::Look();
    *lookOp = Objects::Operation::Look::Instantiate();
    lookOp->SetFrom(c_client->c_characterId);
    send(lookOp);
}

void AtlasConnection::Handler(SelectSet * set)
{
    c_client->Poll();
}
