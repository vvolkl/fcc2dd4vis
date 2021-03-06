
#include <iostream>
#include <vector>

#include "DDG4/DDG4Dict.h"
#include "DDG4/Geant4Data.h"
#include "DDG4/Geant4Hits.h"
#include "DDEve/DDEveEventData.h" 
#include "DDEve/Dictionary.h" 

// podio specific includes
#include "podio/EventStore.h"
#include "podio/ROOTReader.h"

#include "datamodel/TrackClusterCollection.h"
#include "datamodel/TrackHitCollection.h"
#include "datamodel/CaloClusterCollection.h"
#include "datamodel/CaloHitCollection.h"

#include "TSystem.h"
#include "TBrowser.h"
#include "TFile.h"
#include "TTree.h"
#include "TEveManager.h"
#include "TInterpreter.h"
#include "TApplication.h"

using DD4hep::Geometry::Position;
using DD4hep::Simulation::Geant4Tracker;

int main(int argc, char ** argv) {
  auto store = podio::EventStore();
  auto reader = podio::ROOTReader();

  std::string inFilename = "output.root";
  std::cout<<"Read FCCSW data file '"<< inFilename <<"' ..."<<std::endl;
  reader.openFile(inFilename);

  gSystem->Load("libDDG4IO");
  gSystem->Load("libDDEve");
  std::vector<DD4hep::Simulation::Geant4Tracker::Hit*> hv;
  std::vector<DD4hep::Simulation::Geant4Calorimeter::Hit*> chv;
  auto p = new Position(1,2,3);
  
  TFile* f = new TFile("fccvis_converted.root", "RECREATE");
  TTree* T = new TTree("EVENT", "Converted event tree");
  //T->SetAutoSave(0);
  T->Branch("clusters", &hv);
  T->Branch("caloClusters", &chv);

  store.setReader(&reader);
  unsigned nevents= reader.getEntries();
  std::cout<<"found "<<nevents<<" events ..."<<std::endl;
  for(unsigned iev=0; iev<nevents; ++iev) {
    const fcc::TrackClusterCollection* clustercoll(nullptr);
    bool clusterPresent = store.get("clusters", clustercoll);
    if (clusterPresent) {
      for(const auto& cluster : *clustercoll){
        DD4hep::Simulation::Geant4Tracker::Hit* hd = new DD4hep::Simulation::Geant4Tracker::Hit();
        fcc::BareCluster core = cluster.Core();
        p->SetX(core.position.X);
        p->SetY(core.position.Y);
        p->SetZ(core.position.Z);
        hd->position = *p;
        hd->energyDeposit = core.Energy;
        // time not used in dd4hep format
        // direction (momentum) of hit not provided by edm 
        hv.push_back(hd);
      }
    }
    const fcc::CaloClusterCollection* caloClustercoll(nullptr);
    bool caloClusterPresent = store.get("caloClusters", caloClustercoll);
    if (caloClusterPresent) {
      for(const auto& caloCluster : *caloClustercoll){
        DD4hep::Simulation::Geant4Calorimeter::Hit* chd = new DD4hep::Simulation::Geant4Calorimeter::Hit();
        fcc::BareCluster core = caloCluster.Core();
        p->SetX(core.position.X);
        p->SetY(core.position.Y);
        p->SetZ(core.position.Z);
        chd->position = *p;
        chd->energyDeposit = core.Energy;
        // time not used in dd4hep format
        // direction (momentum) of hit not provided by edm 
        chv.push_back(chd);
      }
    }
    T->Fill();
    hv.clear();
    chv.clear();
    store.clear();
    reader.endOfEvent();
  }
  T->Write();
  delete f;

  char* xmlFilename = argv[1];
  TApplication app("test_app", &argc, argv);
  DD4hep::DDEve::run(xmlFilename);
  app.Run();
  
  return 0;
}
