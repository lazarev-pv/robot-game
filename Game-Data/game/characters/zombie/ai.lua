
function _onSeeBegin(self, target)
    local targetX = ai_getPosition(target)
    local selfX = ai_getPosition(self)
    ai_move(self,selfX<=targetX);
end

function _onSeeEnd(self, target)
    ai_stop(self)
end

function _onContactEnd(self, target)
    ai_attack(self,false,0)
end

function _onContactBegin(self, target)
    ai_stop(self)
    ai_attack(self,true,0)
end

function _onHitted(self, target)
    if ai_getHealth(self) < (ai_getMaxHealth(self) / 2) then
       -- run away
        ai_attack(self,false,0);
        local targetX = ai_getPosition(target)
        local selfX = ai_getPosition(self)

        ai_move(self,selfX>targetX);
    end
end