/*
ClayClient.cpp
------------------
begin           : 1999.12.18
copyright       : (C) 1999 by Aloril
email           : aloril@iki.fi
*/

#include "Net_atlas.h"
#include "Shell.h"
//#include <stdiostream.h>
#include <iostream>
#include <stdlib.h>

#ifdef _WIN32
#define sleep(x) Sleep(x*1000)
#define usleep(x) Sleep(x/1000)
#else
#include <sys/time.h>
#include <unistd.h>
#endif

void ClayClient::soundOperation(const Atlas::Object& op)
{
  int result, result1;
  Atlas::Object arg0,ent;
  result1=parseOp1Arg(op,"sound",arg0);
  //DisplayMessage(arg0);
  result=parseOp1Arg(arg0,"talk",ent);
  if (result) {
    string from, say, to;
    arg0.get("from", from);
    arg0.get("to", to);
    ent.get("say", say);
    c_shell->
      output(from + " said \"" + say + "\" to " + to + "\n");
  } else {
    DisplayMessage(arg0);
  }
}

void AtlasConnection::Say(string statement)
{
  Atlas::Object say = c_client->createEntity(Atlas::A("say", statement),
					     AEND);

  Atlas::Object talkOp = 
    c_client->createOperation("talk",
			      Atlas::A("from", c_client->c_characterId),
			      Atlas::A(say),
			      AEND);

  c_client->sendMsg(talkOp);
}


