function trades = strat(distFromFV, phases, buyLevels, sellLevels, numLevels)

trades = zeros(size(phases));

currLevel = numLevels+1;
pos = 0;

for i = 2:length(phases)
    phase = phases(i);
    t=0;
    if phase > 0
        if distFromFV(i,2) <= buyLevels(currLevel)
            [currLevel, t] = doBuys(distFromFV(i,2),buyLevels,currLevel,numLevels,phase, i);
        end
        if t==0 && distFromFV(i,1) >= sellLevels(currLevel)
            [currLevel, t] = doSells(distFromFV(i,1),sellLevels,currLevel,numLevels,phase);
        end
        pos = pos + t;
        trades(i) = t;
    elseif phases(i-1) == 1
        %disp(['Liquidating ' num2str(pos) ' at ' datestr(localTimes(i))])
        % Liquidate Now!!!
        trades(i) = -pos;
        currLevel = numLevels+1;
        pos = 0;
    end
end

end

function [newLevel, trade] = doSells(distFromFV,sellLevels,currLevel,numLevels,phase)
newLevel = currLevel;
while distFromFV >= sellLevels(newLevel) && newLevel <= numLevels * 2
    if phase == 1 && newLevel > numLevels
        break;
    end
    newLevel = newLevel+1;
end
trade = currLevel - newLevel;

end
function [newLevel, trade] = doBuys(distFromFV,buyLevels,currLevel,numLevels,phase, i)
newLevel = currLevel;
while distFromFV <= buyLevels(newLevel) && newLevel > 1 %FIXME: we don;t get to long numLevels.
    if phase == 1 && newLevel <= numLevels+1
        break;
    end
    newLevel = newLevel-1;
end
trade = currLevel - newLevel;
end
