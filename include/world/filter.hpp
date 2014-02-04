#ifndef ENNOVIA_FILTER_HPP
#define ENNOVIA_FILTER_HPP

#include <boost/function.hpp>
#include "world/locatable.hpp"

namespace Ennovia {
namespace Filter {

void LocatablesOnMap(Map* map,boost::function<void (Locatable*)> handler) {
    typedef std::set<Locatable*>::iterator Iterator;
    for(Iterator i = map->locatables.begin(); i != map->locatables.end(); ++i) {
        if((*i)->getPosition().map == map) {
            handler(*i);
        }
    }
}

}
}
#endif
