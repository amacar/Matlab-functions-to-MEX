#include "mex.h"

/* The gateway function */
void mexFunction(int nlhs, mxArray *plhs[],
                 int nrhs, const mxArray *prhs[])
{
    if(nrhs!=1) {
        mexErrMsgIdAndTxt("MyToolbox:arrayProduct:nrhs","One input required.");
    }
    
    if(nlhs!=1) {
        mexErrMsgIdAndTxt("MyToolbox:arrayProduct:nlhs","One output required.");
    }
    
    double *lasts;
    long length = mxGetM(prhs[0]);
    double *bidAsks = mxGetPr(prhs[0]);
    double nanVal = mxGetNaN();
    
    plhs[0] = mxCreateDoubleMatrix((mwSize)length, 1, mxREAL);
    lasts = mxGetPr(plhs[0]);
    lasts[0]=bidAsks[0];
    long i;
    double prev, bid, ask;
    for(i = 1; i < length; i++)
    {
        lasts[i]=nanVal;
        prev=lasts[i-1];
        bid=bidAsks[i];
        ask=bidAsks[i+length];
        if(mxIsNaN(bid))
            lasts[i] = prev;
        else if(mxIsNaN(prev))
            lasts[i] = bid;
        else if(bid >= prev)
            lasts[i] = bid;
        else if(ask <= prev)
            lasts[i] = ask;
        else
            lasts[i] = prev;
    }
}
