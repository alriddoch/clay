#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

using namespace std;

#include "Net.h"
#include "Net_atlas.h"
#include "Shell.h"
#include "Command.h"

#include "Module_Connect.h"
#include "Module_Objects.h"
#include "Module_Communicate.h"

int main(int argc, char ** argv)
{

  #define TEST_ADDR "www.worldforge.net"  // cyphesis
  
  //#define TEST_ADDR "127.0.0.1"  // cyphesis
  //#define TEST_ADDR "210.9.179.126" // chatworld


    SelectSet * dset = new SelectSet;
    //    Descriptor * aconn = new AtlasConnection("127.0.0.1");
    TermShell * in = TermShell::Instance();
    in->setup_screen(24, 80, 1, "Clay Connection test");
    //    in->setup_screen(0, 80, 1, "Clay Connection test"); // no fun stuff

    dset->Register(in);

    Command::Gatherer *g = new Command::Gatherer();
    g->initialize("root");

    Command::Module *cm_obj = new Module_Objects();
    cm_obj->initialize("object");
    cm_obj->registerIn(g);

    Command::Module *cm_conn = new Module_Connect(dset);
    cm_conn->initialize("connect");
    cm_conn->registerIn(g);

    Command::Module *cm_comm = new Module_Communicate();
    cm_comm->initialize("communicate");
    cm_comm->registerIn(g);

    assert(g);
    in->setGatherer(g);
    cerr << "ergh\n";
    while(1){
        dset->Select();
    }

}
