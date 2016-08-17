#ifndef DD4HEP_DDEVE_FCCEVENTHANDLER_H
#define DD4HEP_DDEVE_FCCEVENTHANDLER_H

// Framework include files
#include "DDEve/EventHandler.h"

#include "podio/EventStore.h"
#include "podio/ROOTReader.h"


#include <map>
#include <string>


namespace DD4hep {

  class FCCEventHandler : public EventHandler  {
  public:
    //typedef std::map<std::string,lcio::LCCollection*> Branches;
    //typedef const void* (*HitAccessor_t)(const lcio::LCObject*, DDEveHit*);
  protected:
    /// Reference to data file reader
    //lcio::LCReader* m_lcReader;
    //lcio::LCEvent*  m_event;
    podio::EventStore m_podioStore = podio::EventStore();
    podio::ROOTReader m_ROOTReader = podio::ROOTReader();


    std::string     m_fileName;
    /// Branch map
    //Branches        m_branches;
    /// Data collection map
    TypedEventCollections m_data;
    bool m_hasFile;
    bool m_hasEvent = 1;
    int m_event;
  public:
    /// Standard constructor
    FCCEventHandler();
    /// Default destructor
    virtual ~FCCEventHandler();

    //virtual bool hasEvent() { std::cout << "has Event" <<std::endl; return true;};
    /// Access the map of simulation data collections
    virtual const TypedEventCollections& data()  const   { return m_data;  }
    /// Access the data source name
    std::string datasourceName() const { return hasFile() ? m_fileName : std::string("UNKNOWN"); }
    /// Access the number of events on the current input data source (-1 if no data source connected)
    virtual long numEvents() const;
    /// Access to the collection type by name
    virtual CollectionType collectionType(const std::string& collection) const;
    /// Call functor on hit collection
    virtual size_t collectionLoop(const std::string& collection, DDEveHitActor& actor);
    /// Loop over collection and extract particle data
    virtual size_t collectionLoop(const std::string& collection, DDEveParticleActor& actor);
    /// Open new data file
    virtual bool Open(const std::string& type, const std::string& file_name);
    /// User overloadable function: Load the next event
    virtual bool NextEvent();
    /// User overloadable function: Load the previous event
    virtual bool PreviousEvent();
    /// Goto a specified event in the file
    virtual bool GotoEvent(long event_number);
  };

} /* End namespace DD4hep   */

#endif /* DD4HEP_DDEVE_FCCEVENTHANDLER_H */

