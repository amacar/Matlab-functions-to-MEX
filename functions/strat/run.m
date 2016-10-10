
load sampleInputs.mat
mex stratC.c;

% old code
%tic
%trades = strat(distFromFV, phases, buyLevels, sellLevels, numLevels);
%toc

% C code
tic
trades1 = stratC(distFromFV, phases, buyLevels, sellLevels, numLevels);
toc