#ifndef ENNOVIA_CORE_HPP
#define ENNOVIA_CORE_HPP

#include "misc/irrlicht.hpp"
#include <boost/shared_ptr.hpp>
#include <sstream>

using namespace boost;

namespace Ennovia {

static const int TICK = 100;
static const int FRAME = 30;
static const int PORT = 57634;
static const std::string PORT_S = "57634";

template <class X>
struct All {
    bool operator()(X& x) { return true; }
};


}

#endif
