
class 'PotionEffect'

function PotionEffect:__init()
end

function PotionEffect:apply(target, quality)
end

class 'PotionEffectF' (PotionEffect)

function PotionEffectF:__init(f)
    PotionEffect.__init(self)
    self.f = f
end

function PotionEffectF:apply(target, quality)
    f(target, quality)
end

PotionEffects = {}
PotionEffects[""] = PotionEffect()
PotionEffects["CurePoison"] = PotionEffectF(
    function(target, quality)
        -- search for poison buff on target and clean it
    end
)

function Substance(prim, sec)
    return { primary = prim, secondary = sec }
end

Medicol = Substance("Cure", "HP")
Acetol = Substance("Apply", "Poison")
Jutin = Substance("Adapt", 1)

Blessweed = Herb(Medicol,1,Acetol,4)

class 'Herb' (ItemType)
function Herb:__init(sub1,shp1,sub2,shp2)
    ItemType.__init(self)
    self.sub = { sub1, sub2 }
    self.shape = { shp1, shp2 }
end

function Herb:onUse(user,target)
    if target.type == "Potion" then
        target.quality = math.min(target.quality, self.quality)
        for i=1,2 do
            for j=1,4 do
                if self.shape[i] == target.shape[j] then
                    if sub[i].primary == "Adapt" then
                        target.shape[j] = self.sub[i].secondary
                    else
                        target.sub[j] = self.sub[i]
                        target.shape[j] = 0
                    end
                    break
                end
            end
        end
    end
end

class 'Potion' (ItemType)
function Potion:__init()
    ItemType.__init(self)
    self.shape = { 1,2,3,4 }
    self.sub = {}
    self.quality = 100
end

function Potion:onUse(user,target)
    function GetEffect(sub1, sub2)
        if sub1 and sub2 then
            result = PotionEffects[sub1.primary .. sub2.secondary]
        end
        if not result then
            result = PotionEffects[""]
        end
        return result
    end
    GetEffect(self.sub[1], self.sub[4]).apply(target)
    GetEffect(self.sub[3], self.sub[2]).apply(target)
end
