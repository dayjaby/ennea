#include "world/player.hpp"
#include "world/optionlist.hpp"

namespace Ennovia {
    /// Player

    Player::Player(const Position& position, const std::string& name_)
        : LivingObject(position, name_), currentItem(0) {
    }

    bool Player::hasCurrentItem() {
        return currentItem;
    }


    OptionList* Player::getOptionList(LivingObject* lo) {
        OptionList* ol = LivingObject::getOptionList(lo);
        return ol;
    }

    void Player::onGainExp(std::string skillname, int amount) {
        	/*stat.exp = stat.exp + amount
	self:onGainExp(stat, amount)
	local x = stat.max
	while stat.exp > stat.exp_required do
		stat.max = stat.max + 1
		stat.current = stat.current + 1
		stat.exp = stat.exp - stat.exp_required
		stat.exp_required = stat.exp_required * 1.1
		self:onLevelup(stat)
	end
	return stat.max - x;*/
    }
    void Player::onLevelup(std::string skillname, int lvl) {}

}
