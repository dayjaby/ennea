#ifndef ENNOVIA_ACTIONS_HPP
#define ENNOVIA_ACTIONS_HPP

#include <map>
#include <boost/shared_ptr.hpp>
#include <string>
#include <utility>
#include <vector>
#include <deque>
#include <iostream>

namespace Ennovia {

    class Entity;
    class Player;

    class Action
    {
    public:
        virtual int getTicks();
        virtual void onPerform(Entity* performer);
        virtual void onFinish(Entity* performer) {}
        bool idle;
    };

    class Option {
    public:
        virtual std::string getDescription();
        virtual void onChoose(Player* player);
    };

    class OptionList
    {
    public:
        OptionList& add(Option* option);
        OptionList& addList(const OptionList& other);
        typedef std::vector<boost::shared_ptr<Option> > Options;
        Options options;
    };


    template <class Obj, class Type = Obj>
    class Examine : public Option
    {
    public:
        Examine(Obj* obj_, Type* type_) :
            obj(obj_), type(type_) {}
        virtual std::string getDescription() {
            return std::string("Examine ") + type->getName();
        }
        virtual void onChoose(Player* performer) {
            std::cout << type->onExamine(obj) << std::endl;
        }
    private:
        Obj* obj;
        Type* type;
    };


}

#endif
