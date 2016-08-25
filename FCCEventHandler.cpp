
#include "DD4hep/Printout.h"
#include "DD4hep/Objects.h"
#include "DD4hep/Factories.h"

#include "podio/EventStore.h"
#include "podio/ROOTReader.h"

#include "datamodel/TrackClusterCollection.h"
#include "datamodel/TrackHitCollection.h"
#include "datamodel/CaloClusterCollection.h"
#include "datamodel/CaloHitCollection.h"
#include "datamodel/GenVertexCollection.h"
#include "datamodel/MCParticleCollection.h"

#include "TSystem.h"
#include "TGMsgBox.h"

#include <stdexcept>
#include <climits>

#include "FCCEventHandler.h"

using namespace DD4hep;
using namespace DD4hep::Geometry;


static void* _create(const char*)  {
  FCCEventHandler* h = new FCCEventHandler();
  return h;
}

DECLARE_CONSTRUCTOR(DDEve_FCCEventHandler,_create)

/// Standard constructor
FCCEventHandler::FCCEventHandler() : EventHandler()  {
}

/// Default destructor
FCCEventHandler::~FCCEventHandler()   {
}

/// Access the number of events on the current input data source (-1 if no data source connected)
long FCCEventHandler::numEvents() const {
  if ( m_hasFile )  {
    return m_ROOTReader.getEntries();
  }
  return -1;
}

/// Access to the collection type by name
EventHandler::CollectionType FCCEventHandler::collectionType(const std::string& cl ) const {
  if ( cl == "caloClusters" ) return CALO_HIT_COLLECTION;
  else if ( cl == "clusters" ) return TRACKER_HIT_COLLECTION;
  else if ( cl == "allGenParticles" ) return PARTICLE_COLLECTION;
  else return NO_COLLECTION;
}


/// Call functor on hit collection
size_t FCCEventHandler::collectionLoop(const std::string& collection, DDEveHitActor& actor)   {
  
  /// TODO: figure out how to avoid code duplication
    const podio::CollectionBase* collBase(nullptr);
    const fcc::TrackClusterCollection* clustercoll(nullptr);
    bool clusterPresent = m_podioStore.get(collection, collBase);
    //if (collection == "clusters") {
    if (clusterPresent) {
    clustercoll = dynamic_cast<const fcc::TrackClusterCollection*>(collBase);
    std::cout<< clustercoll << std::endl;
    if (nullptr != clustercoll) {
      int collsize = clustercoll->size();
      actor.setSize(collsize);
        for(const auto& cluster : *clustercoll){
          fcc::BareCluster core = cluster.Core(); 
          DDEveHit* hit = new DDEveHit(11, core.position.X, core.position.Y, core.position.Z, core.Energy);
          actor(*hit);
        }
    return collsize;
  } else  {
    const fcc::CaloClusterCollection* clustercoll(nullptr);
    bool clusterPresent = m_podioStore.get(collection, clustercoll);
      if (clusterPresent) {
      int collsize = clustercoll->size();
      actor.setSize(collsize);
        for(const auto& cluster : *clustercoll){
          fcc::BareCluster core = cluster.Core(); 
          // first argument is some particle id, set to random value because not needed
          DDEveHit* hit = new DDEveHit(11, core.position.X, core.position.Y, core.position.Z, core.Energy);
          actor(*hit);
        }
    return collsize;
    }
  }
  }
    return 0;
}

/// Loop over collection and extract particle data
size_t FCCEventHandler::collectionLoop(const std::string& collection, DDEveParticleActor& actor)    {
  return 0;
  #if 0
  if (collection == "allGenParticles") {
    const fcc::MCParticleCollection* clustercoll(nullptr);
    bool clusterPresent = m_podioStore.get(collection, clustercoll);
      if (clusterPresent) {
      int collsize = clustercoll->size();
      actor.setSize(collsize);
        for(const auto& cluster : *clustercoll){
          fcc::BareParticle core = cluster.Core(); 
          fcc::ConstGenVertex startVertex = cluster.StartVertex(); 
          fcc::ConstGenVertex endVertex = cluster.EndVertex(); 
          DDEveParticle* p = new DDEveParticle();
					p->id = core.Bits;
					p->vsx = startVertex.Position().X;
					p->vsy = startVertex.Position().Y;
					p->vsz = startVertex.Position().Z;
					p->vex = endVertex.Position().X;
					p->vey = endVertex.Position().Y;
					p->vez = endVertex.Position().Z;
					p->psx = core.P4.Px;
					p->psy = core.P4.Py;
					p->psz = core.P4.Pz;
					p->pdgID     = core.Type;
					p->parent    = 1;
					p->energy    = (core.P4.Px * core.P4.Px + core.P4.Py * core.P4.Py + core.P4.Pz * core.P4.Pz +  core.P4.Mass * core.P4.Mass);
					p->time      = 0;
          actor(*p);
        }
    return collsize;
    }
    return 0;
  }
  return 0;
  #endif 
}

/// Open new data file
bool FCCEventHandler::Open(const std::string&, const std::string& name)   {
  m_hasFile = false;
  m_hasEvent = true;
  m_ROOTReader.openFile(name);
  m_podioStore.setReader(&m_ROOTReader);
  m_hasFile = true;
  return true;
}

/// Load the next event
bool FCCEventHandler::NextEvent()   {
  if (m_hasEvent) {
    m_podioStore.clear();
    m_ROOTReader.endOfEvent();
  }
  m_hasEvent = true;
  m_data.clear();
  // TODOL set
  m_data["clusters"].push_back(std::make_pair("clusters", 200));
  m_data["caloClusters"].push_back(std::make_pair("caloClusters", 200));
  m_data["allGenParticles"].push_back(std::make_pair("allGenParticles", 200));
  return 1;
}

/// Load the previous event
bool FCCEventHandler::PreviousEvent()   {
  throw std::runtime_error("+++ This version of the reader can only access files sequentially!\n"
                      "+++ Access to the previous event is not supported.");
}

/// Goto a specified event in the file
bool FCCEventHandler::GotoEvent(long /* event_number */)   {
  throw std::runtime_error("+++ This version of the reader can only access files sequentially!\n"
                      "+++ Random access is not supported.");
}
