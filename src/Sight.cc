/*
ClayClient.cpp
------------------
begin           : 1999.12.18
copyright       : (C) 1999 by Aloril
email           : aloril@iki.fi
*/

#include "Net_atlas.h"
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

string emptyString="";

void ClayClient::sightOperation(const Atlas::Object& op)
{
  int result, result1;
  Atlas::Object arg0,ent;
  result1=parseOp1Arg(op,"sight",arg0);
  //DisplayMessage(arg0);
  result=parseOp1Arg(arg0,"move",ent);
  if(result) {
    int i,ok;
    string dbl;
    Atlas::Object vector;
    Atlas::Object loc;
    string id;
    string ref;
    ent.get("id",id);
    c_shell->output("movement by: " + id);
    ent.get("loc",loc);
    ok=loc.get("coords",vector);
    loc.get("ref", ref);
    if(ok) {
      c_shell->output(" coords " + ref + ":");
      for(i=0;i<3;i++) {
        vector.get(i,dbl);
        c_shell->output(" " + dbl);
      }
    }
    ok=loc.get("velocity",vector);
    if(ok) {
      c_shell->output(" velocity:");
      for(i=0;i<3;i++) {
        vector.get(i,dbl);
        c_shell->output(" " + dbl);
      }
    }
    c_shell->output("\n");
    return;
  }
  result=parseOp1Arg(arg0,"fire",ent);
  if(result) {
    int i,ok;
    string dbl;
    Atlas::Object vector;
    Atlas::Object loc;
    c_shell->output("Fire ");
    ent.get("loc",loc);
    ok=loc.get("coords",vector);
    if(ok) {
      c_shell->output("coords: ");
      for(i=0;i<3;i++) {
        vector.get(i,dbl);
        c_shell->output(" " + dbl);
      }
    }
    c_shell->output("\n");
    return;
  }
  result=parseOp1Arg(arg0,"create",ent);
  if(result) {
    string id;
    ent.get("id",id);
    c_shell->output(id + " was created");
    c_shell->output("\n");
    return;
  }
  result=parseOp1Arg(arg0,"delete",ent);
  if(result) {
    string id;
    ent.get("id",id);
    c_shell->output(id + " was deleted");
    c_shell->output("\n");
    return;
  }
  result=parseOp1Arg(arg0,"set",ent);
  if(result) {
    double status;
    string id;
    ent.get("id", id);
    ent.get("status", status);
    if (status < 0.0) {
      c_shell->output(id + " was broken");
      c_shell->output("\n");
    } else {
      c_shell->output(id + " was damaged");
      c_shell->output("\n");
    }
    return;
  }
  string abstract_type;
  arg0.get("abstract_type",abstract_type,emptyString);
  if(abstract_type!="operation") {
    processObjectSight(arg0);
    return;
  }
  c_shell->output("unknown sight (uncomment followindg code to see what it was)\n");
  DisplayMessage(op);
  
}

void ClayClient::processObjectSight(const Atlas::Object& obj)
{
  //we saw entity... not action
  int len,i;
  Atlas::Object op,contains,emptyContains;
  string id;
  emptyContains=Atlas::Object::mkList(0);
  obj.get("id",id);
  c_shell->output("sight of object:" + id);
  //add all id's that this contains to look list
  obj.get("contains",contains,emptyContains);
  len=contains.length();
  c_shell->output(" contains these ids:");
  for(i=0;i<len;i++) {
    contains.get(i,id);
    c_shell->output(" " + id);
    idLookList.push_back(id);
  }
  c_shell->output("\n");
  
  //something left to look?
  if(!idLookList.empty()) {
    //get next id to look
    id=idLookList.front();
    idLookList.pop_front();
    Atlas::Object ent=createEntity(Atlas::A("id",id),AEND);
    op=createOperation("look",Atlas::A(ent),Atlas::A("from",c_characterId),AEND);
    //send it and handle it when it arrives (if it arrives: might have been deleted)
    sendMsg(op);
  }
}

int AtlasConnection::lookWorld()
{
  Atlas::Object op;
  
  //look nothing specifically, should return current 'top' container
  op=c_client->createOperation("look",Atlas::A("from",c_client->c_characterId),AEND);
  c_client->sendMsg(op);
  return 1;
}
