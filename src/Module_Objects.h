#ifndef MODULE_OBJECTS_H
#define MODULE_OBJECTS_H

#include "Command.h"
#include "Net_atlas.h"
#include "Shell.h"

class Module_Objects : public Command::Module{
public:
  Module_Objects() : m_aconn(NULL) { }
  int initialize(string name);
  string run(string cmd, vector<string> args, string cmdLine);
  int registerIn(Command::Gatherer *g);
  string get_name(){
      return m_name;
  }
private:
  int doLook(vector<string> args);
  AtlasConnection *m_aconn;
};

#endif /* MODULE_OBJECTS */
