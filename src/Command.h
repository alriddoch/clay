/* emacs, this is -*- C++ -*- */

#ifndef COMMAND_H
#define COMMAND_H

#include <map>
#include <string>
#include <cassert>
#include <vector>
#include <list>


using std::map;
using std::string;

class AtlasConnection;

namespace Command{
  class Module;
  class Gatherer;

  /**
     Command::Module is an abstract base class that defines an interface
     for command modules.
  */
  class Module{
  public:
    //: initialize the command module as name
    virtual int initialize(string name) = 0;
    //: run a command
    virtual string run(string cmd, vector<string> args, string cmdLine) = 0;
    virtual int registerIn(Gatherer *g) = 0;
    string get_name(){
      return m_name;
    }
  protected:
    string m_name;
  };


  typedef map<string, Module *> module_map;
  typedef map<string, list<Module *> > command_map;
  
  class Gatherer : public Module{
  public:
    //: Constructor.
    //    Gatherer();
    //: initialize the Gatherer.
    int initialize(string name);
    //: run a command in a module.  command can be an empty string.
    //: module and cmdLine cannot.
    string run(string module, vector<string> args, string cmdLine);
    //: register myself in another gatherer
    int registerIn(Gatherer *g);
    //: register a Module with a given name, without commands
    int registerModule(const string &name, Command::Module *cm);
    //: register a Module with a given name, with commands
    int registerModule(const string &name, Command::Module *cm,
		       list<string> commands);
    //: register additional commands for a module
    int registerCommand(const string &command_name,
			const string &command_module);
    //: load an already registered Module
    int loadModule(const string &name);
    //: unload a loaded Module
    int unloadModule(const string &name);
  private:
    friend class Module;
    //: a mapping from names to Modules.
    module_map m_modules;
    //: the loaded modules
    module_map m_loaded_modules;
    //: a mapping from commmand name to a list of Modules.
    command_map m_commands;
  };

  class Shell{
  public:
    //: Run this command. intended to be called by the Descriptor
    int run(string s);
    virtual int output(string s) = 0;    
    AtlasConnection * aconn;
  protected:
    Shell() : aconn(NULL) { }
    virtual vector<string> parse(string s) = 0;
    Gatherer * m_gatherer;
  };
}
#endif /* COMMAND_H */

