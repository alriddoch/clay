#ifndef MODULE_CONNECT_H
#define MODULE_CONNECT_H

#include "Command.h"
#include "Shell.h"
#include "Net_atlas.h"

class SelectSet;

class Module_Connect : public Command::Module {
public:
  Module_Connect(SelectSet * set) : dset(set), m_aconn(NULL) { }
  int initialize(string name);
  int registerIn(Command::Gatherer *g);
  string run(string cmd, vector<string> args, string cmdLine);
private:
  string doConnect(vector<string> args);
  string doLogin(vector<string> args);
  string doCreate();
  AtlasConnection *m_aconn;
  SelectSet  *dset;
};

#endif /* MODULE_CONNECT_H */
