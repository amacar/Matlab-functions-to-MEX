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
    
    double *dataOutput;
    long dataC = mxGetN(prhs[0]);
    long dataR = mxGetM(prhs[0]);
    double *data = mxGetPr(prhs[0]);
    double nanVal = mxGetNaN();
    
    plhs[0] = mxCreateDoubleMatrix((mwSize)dataR, (mwSize)dataC, mxREAL);
    dataOutput = mxGetPr(plhs[0]);
    long c,r,offset, offsetSum;
    for(c = 0; c < dataC; c++)
    {
        offset=c*dataR;
        dataOutput[offset] = data[offset];
        for(r = 1; r < dataR; r++)
        {
            offset++;
            dataOutput[offset] = data[offset];
            if(mxIsNaN(dataOutput[offset]))
            {
                dataOutput[offset] = dataOutput[offset-1];
            }
        }
    }
}
