

#include "FCCEventHandler.h"

#include "DDEve/Display.h"
#include "DDEve/Dictionary.h"
#include "TInterpreter.h"
#include "TApplication.h"
#include "TEveManager.h"


int main(int argc, char ** argv) {

  auto h = new DD4hep::FCCEventHandler();
  h->Open("unused", "output.root");
  std::cout << "found " << h->numEvents() << "events ..." << std::endl; 

  char* xmlFilename = argv[1];
  char* eventFilename = argv[2];
  TApplication app("test_app", &argc, argv);
  DD4hep::EveDisplay(xmlFilename, eventFilename);
  app.Run();

  return 1;
}
