class 'CustomOption' (Option)

function CustomOption:__init(descr,fnc)
    Option.__init(self)
    self.description = descr
    self.customOnChoose = fnc
end
function CustomOption:getDescription()
    return self.description
end
function CustomOption:onChoose(performer)
    self.customOnChoose(performer)
end

