itemtypes = {}


class 'ThrowAway' (Option)
function ThrowAway:__init(item)
  self.item = item
  Option.__init(self)
end

function ThrowAway:getDescription()
  return "Throw away!"
end

function ThrowAway:onChoose(player)
  item.count = 0
end


class 'Coin' (ItemType)

function Coin:__init()
  ItemType.__init(self,"stdCoin","Coin","media/coins.png")
end

function Coin:isStackable()
  return true
end

function Coin:onExamine()
  return "Gold!!!"
end

function Coin:getOptionList(player,item)
  ol = OptionList()
  ol.add(ThrowAway(self))
  return ol
end

itemtypes.stdCoin = Coin()