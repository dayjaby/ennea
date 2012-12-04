class 'SimplePoisonBuff' (Buff)

function SimplePoisonBuff:__init(dot)
    Buff.__init(self)
    self.timeLeft = 60
    self.effectCooldown = 10
    self.dot = dot
end

function SimplePoisonBuff:onEffect(carry)
    carry:damage(DamageType.Poison(), self.dot)
end

class 'Poison' (Ability)

function Poison:__init()
    Ability.__init(self)
end

function Poison:onCast(caster, victim)
    victim:applyBuff(SimplePoisonBuff(10))
end

