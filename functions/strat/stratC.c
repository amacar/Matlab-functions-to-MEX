/*
 * arrayProduct.c - example in MATLAB External Interfaces
 *
 * Multiplies an input scalar (multiplier) 
 * times a 1xN matrix (inMatrix)
 * and outputs a 1xN matrix (outMatrix)
 *
 * The calling syntax is:
 *
 *		outMatrix = arrayProduct(multiplier, inMatrix)
 *
 * This is a MEX-file for MATLAB.
*/

#include "mex.h"

//mexPrintf("%d\n", newLevel); // OK 
void doSells(double distFromFV, double *sellLevels, int *currLevel, int numLevels, double phase, int *t)
{
    int newLevel = *currLevel;
    while(distFromFV >= sellLevels[newLevel-1] && newLevel <= numLevels *2)
    {
        if(phase == 1 && newLevel > numLevels)
            break;
        newLevel = newLevel + 1;
    }
    *t = *currLevel - newLevel;
    *currLevel = newLevel;
}

void doBuys(double distFromFV, double *buyLevels, int *currLevel, int numLevels, double phase, int *t)
{
    int newLevel = *currLevel;
    while(distFromFV <= buyLevels[newLevel-1] && newLevel > 1)
    {
        if(phase == 1 && newLevel <= numLevels +1)
            break;
        newLevel = newLevel - 1;
    }
    *t = *currLevel - newLevel;
    *currLevel = newLevel; 
}

/* The gateway function */
void mexFunction(int nlhs, mxArray *plhs[],
                 int nrhs, const mxArray *prhs[])
{
    if(nrhs!=5) {
        mexErrMsgIdAndTxt("MyToolbox:arrayProduct:nrhs","Five inputs required.");
    }
    
    if(nlhs!=1) {
        mexErrMsgIdAndTxt("MyToolbox:arrayProduct:nlhs","One output required.");
    }
    
    double *trades;
    int numLevels = mxGetScalar(prhs[4]);
    int currLevel = numLevels + 1;
    double pos = 0;
    int lengthPhases = mxGetM(prhs[1]);
    double *distFromFV = mxGetPr(prhs[0]);
    double *phases = mxGetPr(prhs[1]);
    double *buyLevels = mxGetPr(prhs[2]);
    double *sellLevels = mxGetPr(prhs[3]);
    
    plhs[0] = mxCreateDoubleMatrix((mwSize)lengthPhases, 1, mxREAL);
    trades = mxGetPr(plhs[0]);
    
    long i;
    int t;
    double phase;
    int izpisi=0;
    for(i=1; i<lengthPhases; i++)
    {
        phase = phases[i];
        t = 0;
        if(phase>0)
        {
            if(distFromFV[i+lengthPhases] <= buyLevels[currLevel-1])
            {
                doBuys(distFromFV[i+lengthPhases],buyLevels,&currLevel,numLevels,phase,&t);
            }
            if(t==0 && distFromFV[i] >= sellLevels[currLevel-1])
            {
                doSells(distFromFV[i],sellLevels,&currLevel,numLevels,phase,&t);
            }
            
            pos = pos + t;
            trades[i] = t;
        }
        else if(phases[i-1] == 1)
        {
            trades[i] = -pos;
            currLevel = numLevels + 1;
            pos = 0;
        }
    }
    
}
