#ifndef ENNOVIA_OPTIONLIST_HPP
#define ENNOVIA_OPTIONLIST_HPP

#include <string>
#include <vector>
#include "core.hpp"

namespace Ennovia {

    class LivingObject;
    class Option {
    public:
        virtual ~Option() {}
        virtual std::string getDescription();
        virtual void onChoose(LivingObject* lo);
    };

    class OptionList;

    class OptionProxy : public Option {
    public:
        OptionProxy(const std::string& desc_, int id, int index);
        OptionProxy(const OptionProxy& other);
        virtual std::string getDescription();
        virtual void onChoose(LivingObject* lo);

        friend class OptionList;
    private:
        std::string desc;
        int id;
        int index;
    };

    class OptionList
    {
    public:
        bool isValid();
        OptionList& add(Option* option);
        OptionList& addList(const OptionList& other);
        typedef std::vector<boost::shared_ptr<Option> > Options;
        Options options;
    };



}

#endif
