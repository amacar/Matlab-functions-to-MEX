function lasts = getSyntheticLasts(bidAsks)
%Aims to reduce the volatility of mid-points. Takes bidAsks
lasts = nan(size(bidAsks,1),1);

lasts(1) = bidAsks(1,1);
for i = 2:length(lasts)
    prev = lasts(i-1);
    bid = bidAsks(i,1);
    ask = bidAsks(i,2);
    if isnan(bid)
        lasts(i) = prev;
    elseif isnan(prev)
        lasts(i) = bid;
    elseif bid >= prev
        lasts(i) = bid;
    elseif ask <= prev
        lasts(i) = ask;
    else
        lasts(i) = prev;
    end
end
end