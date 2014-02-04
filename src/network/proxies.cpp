#include "network/network.hpp"

namespace Ennovia {
/*
POptionList getPOptionList(const reg<OptionList>& optl)
{
    POptionList pOptl;
    pOptl.set_id(optl.getId());
    for(int i=0;i<optl->options.size();i++)
    {
        POption* opt = pOptl.add_options();
        opt->set_id(i);
        opt->set_desc(optl->options[i]->getDescription());
    }
    return pOptl;
}

reg<OptionList>& getOptionList(const POptionList& pOptl)
{
    return Registry<OptionList>::get(pOptl.id());
}

PPOptionList getPPOptionList(const reg<POptionList>& pOptl)
{
    PPOptionList ppOptl;
    ppOptl.set_id(pOptl.getId());
    return ppOptl;
}

reg<POptionList>& getPOptionList(const PPOptionList& ppOptl)
{
    return Registry<POptionList>::get(ppOptl.id());
}

Option& getOption(const POptionList& pOptList, const POption& pOpt) {
    return *getOptionList(pOptList)->options[pOpt.id()];
}

Option& getOption(const POptionRef& pOpt)
{
    return getOption(pOpt.optionlist(),pOpt.option());
}

PEntity getPEntity(const reg<Entity>& ent)
{
    PEntity pEnt;
    pEnt.set_id(ent.getId());
    return pEnt;
}

reg<Entity>& getEntity(const PEntity& pEnt)
{
    return Registry<Entity>::get().get(pEnt.id());
}

*/

}
