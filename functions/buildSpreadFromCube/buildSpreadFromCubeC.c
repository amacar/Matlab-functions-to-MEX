#include "mex.h"
#include "matrix.h"

/* The gateway function */
void mexFunction(int nlhs, mxArray *plhs[],
                 int nrhs, const mxArray *prhs[])
{
    if(nrhs!=2) {
        mexErrMsgIdAndTxt("MyToolbox:arrayProduct:nrhs","Two inputs required.");
    }
    
    if(nlhs!=1) {
        mexErrMsgIdAndTxt("MyToolbox:arrayProduct:nlhs","One output required.");
    }
    
    double *cube= mxGetPr(prhs[0]);
    double *coefficients = mxGetPr(prhs[1]);
    int *cubeDims = mxGetDimensions(prhs[0]);
    int dim0=cubeDims[0];
    int dim1=cubeDims[1];
    int dim2=cubeDims[2];
    
    int i,j,k;
    int firstAny, secondAny;
    int *openIdxPtr;
    int numberOfopenIdxElements=0;
    mxArray *openIdx = mxCreateNumericMatrix(dim0, 1, mxINT32_CLASS, mxREAL);
    openIdxPtr=mxGetPr(openIdx);
    for(i=0;i<dim0;i++)
    {
        secondAny=1;
        for(j=0;j<dim2;j++)
        {
            firstAny=0;
            int offset=i+j*dim0*dim1;
            for(k=0;k<dim1;k++)
            {
                if(mxIsNaN(cube[offset]))
                {
                    firstAny=1;
                    break;
                }
                offset+=dim0;
            }
            if(firstAny==1)
            {
                secondAny=0;
                break;
            }
        }
        if(secondAny==1)
            openIdxPtr[numberOfopenIdxElements++]=i;
    }
    
    mwSignedIndex logrtsDims[3];
    logrtsDims[0]=numberOfopenIdxElements-1;
    logrtsDims[1]=2;
    logrtsDims[2]=3;
    double *logrtsPtr;
    mxArray *logrts = mxCreateNumericArray(3, logrtsDims, mxDOUBLE_CLASS, mxREAL);
    logrtsPtr = mxGetPr(logrts);
    int offset1, offset2, counter=0, logrtsDims0=logrtsDims[0];
    for(i=1;i<numberOfopenIdxElements;i++)
    {
        for(j=0;j<dim2;j++)
        {
            for(k=0;k<dim1;k++)
            {
                logrtsPtr[counter + k*logrtsDims0+j*logrtsDims0*dim1] = log(cube[(int)openIdxPtr[i]+k*dim0+j*dim0*dim1] / cube[(int)openIdxPtr[i-1]+k*dim0+j*dim0*dim1]);
            }
        }
        counter++;
    }
    
    double initialSpreadWidth=0;
    for(j=0;j<dim2;j++)
    {
        initialSpreadWidth += ((cube[(int)openIdxPtr[0]+dim0+j*dim0*dim1]-cube[(int)openIdxPtr[0]+j*dim0*dim1])/cube[(int)openIdxPtr[0]+j*dim0*dim1])*fabs(coefficients[j]);
    }
    
    double *spreadBidAsksPtr;
    plhs[0] = mxCreateNumericMatrix(dim0, dim1, mxDOUBLE_CLASS, mxREAL);
    spreadBidAsksPtr = mxGetPr(plhs[0]);
    double NaN = mxGetNaN();
    
    for(i=0;i<dim0;i++)
    {
        for(j=0;j<dim1;j++)
        {
            spreadBidAsksPtr[i+j*dim0]=NaN;
        }
    }
    
    spreadBidAsksPtr[(int)openIdxPtr[0]]=1;
    spreadBidAsksPtr[(int)openIdxPtr[0]+dim0]=1+initialSpreadWidth;
    double sum1,sum2;
    for(i=0;i<logrtsDims0;i++)
    {
        sum1=0;
        sum2=0;
        for(j=0;j<dim2;j++)
        {
            if(coefficients[j]>0)
            {
                sum1 += coefficients[j]*logrtsPtr[i + j*logrtsDims0*dim1];
                sum2 += coefficients[j]*logrtsPtr[i + logrtsDims0+j*logrtsDims0*dim1];
            }
            else if(coefficients[j]<0)
            {
                sum1 += coefficients[j]*logrtsPtr[i + logrtsDims0+j*logrtsDims0*dim1];
                sum2 += coefficients[j]*logrtsPtr[i + j*logrtsDims0*dim1];
            }
        }
        spreadBidAsksPtr[(int)openIdxPtr[i+1]] = exp(sum1)*spreadBidAsksPtr[(int)openIdxPtr[i]];
        spreadBidAsksPtr[(int)openIdxPtr[i+1]+dim0] = exp(sum2)*spreadBidAsksPtr[(int)openIdxPtr[i]+dim0];
    }
    
    mxDestroyArray(openIdx);
    mxDestroyArray(logrts);
    //mexPrintf("%d\n",numberOfopenIdxElements);   
}
