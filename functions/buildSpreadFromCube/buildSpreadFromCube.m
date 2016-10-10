function spreadBidAsks = buildSpreadFromCube(cube, coefficients)
openIdx = find(~any(any(isnan(cube),2),3));

logrts = [log(cube(openIdx(2:end),:,:)./cube(openIdx(1:end-1),:,:))];

coefficientCubeBid = zeros(size(logrts,1),2,size(logrts,3));
coefficientCubeBid(:,1,coefficients>0)= repmat(reshape(coefficients(coefficients>0),[1,1,sum(coefficients>0)]),size(logrts,1),1);
coefficientCubeBid(:,2,coefficients<0)= repmat(reshape(coefficients(coefficients<0),[1,1,sum(coefficients<0)]),size(logrts,1),1);

spread = [sum(sum(logrts.*coefficientCubeBid,3),2) sum(sum(logrts.*flipdim(coefficientCubeBid,2),3),2)];
 
initialSpreadWidth = sum(squeeze(diff(cube(openIdx(1),:,:))./cube(openIdx(1),1,:)).*abs(coefficients'));

spread = cumprod([1 1+initialSpreadWidth; exp(spread)]);

spreadBidAsks = nan(size(cube,1),size(cube,2));
spreadBidAsks(openIdx,:) = spread;

end