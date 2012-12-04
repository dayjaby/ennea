
class 'Bucket' (ItemType)
function Bucket:__init()
    ItemType.__init(self,"Bucket","media/coins.png")
end
function Bucket:onExamine(Item)
    return "An old wooden bucket"
end
function Bucket:getOptionList(user,Item)
    al = OptionList()
    if(user:hasCurrentItem() and user.currentItem.type == ItemTypes["WaterBucket"]) then
        al:add(CustomOption("Use Water Bucket on empty Bucket",
            function(performer)
                print("You filled the bucket with water")
                user.currentItem:changeItemType(ItemTypes["Bucket"])
                Item:changeItemType(Items["WaterBucket"])
            end))
    end
    return al
end

class 'WaterBucket' (ItemType)
function WaterBucket:__init()
    ItemType.__init(self,"Water bucket","media/coins.png")
end
function WaterBucket:onExamine(Item)
    return "A bucket filled with water"
end
function WaterBucket:getActionList(user,Item)
    return OptionList():add(CustomOption("Empty bucket",
                                function(performer)
                                    print("You emptied the bucket")
                                    Item:changeItemType(ItemTypes["Bucket"])
                                end))
end

class 'Coins' (ItemType)
function Coins:__init()
    ItemType.__init(self,"Coins","media/coins.png")
end
function Coins:onExamine(item)
    return 'coins'
end

class 'Knife' (ItemType)
function Knife:__init()
    ItemType.__init(self,"Knife","media/coins.png")
end
function Knife:onExamine(Item)
    return "A rusty knife"
end

ItemTypes = { Bucket = Bucket(),
          WaterBucket = WaterBucket(),
          Coins = Coins()
        }
