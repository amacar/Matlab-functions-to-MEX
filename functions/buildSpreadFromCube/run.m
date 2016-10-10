load inputs.mat
mex buildSpreadFromCubeC.c;

% cube=[1 2;3 4; 5 6];
% cube(:,:,2)=[7 8;9 10; 11 12];

%coefficients=[-5,0,-2.5];

tic
% for c = 1:100
spreadBidAsks = buildSpreadFromCube(cube, coefficients);
% end
toc

tic
% for c = 1:200
spreadBidAsks1 = buildSpreadFromCubeC(cube, coefficients);
% end
toc

tic
for c = 1:size(spreadBidAsks,2)
    breakOuterFor=false;
    for r = 1:size(spreadBidAsks,1)
        if abs(spreadBidAsks(r,c)-spreadBidAsks1(r,c)) > 1e4*eps(min(abs(spreadBidAsks(r,c)),abs(spreadBidAsks1(r,c))))
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
toc

% tic
% for c = 1:size(spreadBidAsks,1)
%     breakOuterFor=false;
%     for r = 1:size(spreadBidAsks,2)
%         for e = 1:size(spreadBidAsks,3)
%             if abs(spreadBidAsks(c,r,e)-spreadBidAsks1(c,r,e)) > 1e4*eps(min(abs(spreadBidAsks(c,r,e)),abs(spreadBidAsks1(c,r,e))))
%                 c
%                 r
%                 e
%                 breakOuterFor=true;
%                 break;
%             end
%         end
%         if breakOuterFor==1
%             break;
%         end
%     end
%     if breakOuterFor==1
%         break;
%     end
% end
% toc