#include "world/actions.hpp"
#include "world/livingobject.hpp"
#include "mayor.hpp"

namespace Ennovia {

    Move::Move(Locatable* performer,const Position& start,const Position& end)
    : Action(performer) {
        float dx = end.x - start.x;
        float dy = end.y - start.y;
        distance = std::sqrt(dx*dx+dy*dy);
        float movement = performer->getMovementSpeed();
        if(distance == 0) {
            duration = 0;
            dX = 0;
            dY = 0;
        } else {
            duration = std::ceil(distance / movement);
            dX = dx / duration;
            dY = dy / duration;
        }
        std::cout << "dx=" << dx << ",dy=" << dy << std::endl;
        std::cout << "DURATION=" << duration << ",MOVEMENT=" << movement << ",DX=" << dX << ",DY=" << dY << std::endl;
    }
    void Move::onTick() {
        Position pos = performer->getPosition();
        pos.x += dX;
        pos.y += dY;
        performer->setPosition(pos);
    }
    void Move::onFinish() {
        Mayor& mayor = Mayor::get();
        mayor.getRender().setAnimation(mayor.getLibrary().getSceneNode(performer).get(),irr::scene::EMAT_STAND);
    }

}
