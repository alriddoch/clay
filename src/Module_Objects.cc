#include "Command.h"
#include "Net_atlas.h"
#include "Shell.h"

#include "Module_Objects.h"

int Module_Objects::initialize(string name)
{
  m_name = name;
}

int Module_Objects::registerIn(Command::Gatherer *g)
{
  assert(m_name != "");
  g->registerModule(m_name, (Command::Module *) this);
  g->registerCommand("look", m_name);
}

string Module_Objects::run(string cmd, vector<string> args, string cmdLine)
{
  if(!m_aconn){
    if(TermShell::Instance()->aconn){
      m_aconn = TermShell::Instance()->aconn;    
    }else{
      return("Connect first.\n");
    }
  }

  if(cmd == "look"){
    doLook(args);
    return("looking\n");
  }else{
    return("Connect Module commands:\n\tlook\n\n");
  }
}

int Module_Objects::doLook(vector<string> args)
{
  assert(m_aconn);
  if(args.size() > 1){
    // look at an object
  }else{
    // plain old look
    m_aconn->lookWorld();
  }
}
