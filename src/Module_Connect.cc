#include "Module_Connect.h"
#include "Descriptor.h"

int Module_Connect::initialize(string name)
{
  m_name = name;
}

int Module_Connect::registerIn(Command::Gatherer *g)
{
  assert(m_name != "");
  assert(g);

  g->registerModule(m_name, (Command::Module *) this);
  g->registerCommand("connect", m_name);
  g->registerCommand("disconnect", m_name);
  g->registerCommand("login", m_name);
  g->registerCommand("createcharacter", m_name);
}

string Module_Connect::run(string cmd, vector<string> args, string cmdLine)
{
  cerr << "M_C::run: " << cmd << "\n";
  if(cmd == "connect"){
    return(doConnect(args));
  }else if(cmd == "disconnect"){
    return("Not implemented yet\n");    
  }else if(cmd == "login"){
    return(doLogin(args));
  }else if(cmd == "createcharacter"){
    return(doCreate());
  }else{
    return("Connect Module commands:\n\tconnect\n\tdisconnect\n\tlogin\n\tcreatecharacter\n\n");
  }
}

string Module_Connect::doConnect(vector<string> args){
  if(m_aconn){
    return("Already Connected\n");
  }
  if(TermShell::Instance()->aconn){
    m_aconn = TermShell::Instance()->aconn;
    return("Already Connected\n");
  }

  string h;
  int p;

  if(args.size() == 1){
    h = "127.0.0.1"; p = 6767;
  }else if(args.size() == 2){
    h = args[1]; p = 6767;
  }else{
    h = args[1]; p = atoi(args[2].c_str());
  }

  AtlasConnection * aconnect = new AtlasConnection(TermShell::Instance());
  if (aconnect->setup(h, p) == 0) {
    TermShell::Instance()->aconn = m_aconn = aconnect;
    dset->Register(aconnect);
    return("Connected\n");
  }
  return("Connect failed\n");
}

string Module_Connect::doLogin(vector<string> args)
{
  if(!m_aconn){
    return("connect first\n");
  }

  if(args.size() != 3){
    return("usage: login username password\n");
  }

  if(m_aconn->login(args[1], args[2]) == 0){
    return("Logged in\n");
  }else{
    return("Login failed\n");
  }
}

string Module_Connect::doCreate()
{
  if(!m_aconn){
    return("connect first\n");
  }

  if(m_aconn->createCharacter("Cally Claymore") == 0){
    return("Created character\n");
  }else{
    return("Creating character failed\n");
  }
}

