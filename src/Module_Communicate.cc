#include "Module_Communicate.h"

int Module_Communicate::initialize(string name)
{
  m_name = name;
}

int Module_Communicate::registerIn(Command::Gatherer *g)
{
  assert(g);
  assert(m_name != "");
  g->registerModule(m_name, (Command::Module *) this);
  g->registerCommand("say", m_name);
}

string Module_Communicate::run(string cmd, vector<string> args, string cmdLine)
{
  if(!m_aconn){
    if(TermShell::Instance()->aconn){
      m_aconn = TermShell::Instance()->aconn;    
    }else{
      return("Connect first.\n");
    }
  }

  if(cmd == "say"){
    doSay(cmd, cmdLine);
    return("Saying...\n");
  }else{
    return("Communicate Module commands:\n\tsay\n\n");
  }
}

int Module_Communicate::doSay(string cmd, string cmdLine)
{
  int end_cmd, start_statement;  
  string statement;

  cerr << "saying: " << cmd << " : " << cmdLine << "\n";

  end_cmd = cmdLine.find(cmd) + cmd.size();
  start_statement = cmdLine.find_first_not_of(" \t\r\n", end_cmd);

  assert(start_statement > end_cmd);

  statement = string(cmdLine, start_statement);

  cerr << "saying: " << statement << "\n";

  assert(m_aconn);

  m_aconn->Say(statement);
}
