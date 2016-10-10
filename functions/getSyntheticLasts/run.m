load inputs.mat
mex getSyntheticLastsC.c;

tic
lasts = getSyntheticLasts(data);
toc

tic
lasts1 = getSyntheticLastsC(data);
toc

for c = 1:size(lasts,2)
    breakOuterFor=false;
    for r = 1:size(lasts,1)
        if lasts(r,c) ~= lasts1(r,c)
            r
            c
            breakOuterFor=true;
            break;
        end
    end
    if breakOuterFor==1
        break;
    end
end