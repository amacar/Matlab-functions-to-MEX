load inputs.mat
mex fillFwdNansC.c;

tic
d1 = fillFwdNans(data);
toc

tic
d2 = fillFwdNansC(data);
toc

for c = 1:size(d1,2)
    breakOuterFor=false;
    for r = 1:size(d1,1)
        if d1(r,c) ~= d2(r,c)
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

