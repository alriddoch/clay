#include "Command.h"

int Command::Gatherer::initialize(string name)
{
  m_name = name;
}

string Command::Gatherer::run(string module, vector<string> args,
			      string cmdLine)
{
  Command::Module *mod;
  command_map::iterator cmi;
  module_map::iterator mi;
  string cmd = "";

  cerr << "C::G::run: " << args[0] << "\n";

  cerr << "0";

  if(module == ""){
    cerr << "1";
    cmi = m_commands.find(args[0]);
    cerr << "A";
    if(cmi == m_commands.end()){
      cerr << "2";
      /* no such command */
      return("Command not found\n");
    }else{
      cerr << "3";
      /* found a command, get first element in list of modules */
      mod = *(cmi->second.begin());
      cmd = cmi->first;
    }
  }else{
    mi = m_modules.find(module);
    if(mi == m_modules.end()){
      return("Module not found\n");
    }else{
      /* found the module */
      mod = mi->second;
    }
  }
  return mod->run(cmd, args, cmdLine);
}

int Command::Gatherer::registerIn(Gatherer *g)
{
  return(0);
}

int Command::Gatherer::registerModule(const string &name, Command::Module *cm)
{
  if(m_modules[name]){
    /* error if it's already registered*/
    return(0);
  }
  m_modules[name] = cm;
  return(1);
}

int Command::Gatherer::registerModule(const string &name, Command::Module *cm,
				      list<string> commands)
{
  if(m_modules[name]){
    /* error if it's already registered*/
    return(0);
  }
  m_modules[name] = cm;
  for(list<string>::iterator command = commands.begin();
      command != commands.end();
      command++)
    m_commands[*command].push_front(cm);
  return(1);
}

int Command::Gatherer::registerCommand(const string &command_name,
				       const string &command_module)
{
  module_map::iterator mi;
  mi = m_modules.find(command_module);

  if(mi == m_modules.end()){
    /* command_module is not yet registered */ 
    return(0);
  }
  
  /* push this command module to the front of the module list */
  m_commands[command_name].push_front(mi->second);

  /* XXX - STL magic makes this always work? */
  return(1);
}

#if COMMENTED_OUT
/* stuff below tests gatherer */

class TrivialModule1 : public Command::Module{
  int initialize(string name){
    m_name = name;
  }
  int registerIn(Command::Gatherer *g){
    g->registerModule(m_name, (Command::Module *) this);
    g->registerCommand("one", m_name);
    g->registerCommand("two", m_name);
    g->registerCommand("three", m_name);  
  }
  string run(string cmd, vector<string> args, string cmdLine){
    return("1: " + args[0]);
  }
};

class TrivialModule2 : public Command::Module{
  int initialize(string name){
    m_name = name;
  }
  int registerIn(Command::Gatherer *g){
    g->registerModule(m_name, (Command::Module *) this);
    g->registerCommand("one", m_name);
    g->registerCommand("two", m_name);
    g->registerCommand("three", m_name);
  }
  string run(string cmd, vector<string> args, string cmdLine){
    return("2: " + args[0]);
  }
};

int main()
{
  Command::Module *tm1 = new TrivialModule1();
  Command::Module *tm2 = new TrivialModule2();
  Command::Gatherer *g;

  vector<string> one(1, "one");
  vector<string> two(2, "two");
  vector<string> three(3, "three");
  vector<string> four(4, "four");

  g = new Command::Gatherer;
  g->init();
  tm2->initialize("trivial2");
  tm1->initialize("trivial1");
  tm1->registerIn(g);
  tm2->registerIn(g);


  cerr << "trivial1 one: " << g->run("trivial1", one, "") << "\n";
  cerr << "trivial2 two: " << g->run("trivial2", two, "") << "\n";
  cerr << "trivial1 three: " << g->run("trivial1", three, "") << "\n";
  cerr << "trivial2 four: "  << g->run("trivial2", four, "") << "\n";
  cerr << "trivial1 three: "  << g->run("trivial1", three, "") << "\n";
  cerr << "trivial2 four: "  << g->run("trivial2", four, "") << "\n";
  cerr << "nontrivial one: " << g->run("nontrivial", one, "") << "\n";
  cerr << "nontrivial four: " << g->run("nontrivial", four, "") << "\n";
  cerr << "[] one: " << g->run("", one, "") << "\n";
  cerr << "[] four: " << g->run("", four, "") << "\n";
}
#endif /* Commented out */
