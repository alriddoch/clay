#ifndef MODULE_COMMUNICATE_H
#define MODULE_COMMUNICATE_H

#include "Command.h"
#include "Net_atlas.h"
#include "Shell.h"

class Module_Communicate : public Command::Module{
public:
  Module_Communicate() : m_aconn(NULL) { }
  int initialize(string name);
  string run(string cmd, vector<string> args, string cmdLine);
  int registerIn(Command::Gatherer *g);
  string get_name(){
      return m_name;
  }
private:
  int doSay(string cmd, string cmdLine);
  AtlasConnection *m_aconn;
};

#endif MODULE_COMMUNICATE_H
