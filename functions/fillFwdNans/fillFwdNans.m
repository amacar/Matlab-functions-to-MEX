function data = fillFwdNans(data)

for c = 1:size(data,2)
for r = 2:size(data,1)
    if isnan(data(r,c)),
        data(r,c) = data(r-1,c);
    end
end

end