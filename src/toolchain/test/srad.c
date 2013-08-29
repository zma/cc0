long rows, cols, size_I, size_R, niter, iter, k;
double *I, *J, q0sqr, sum, sum2, tmp, meanROI, varROI ;
double Jc, G2, L, num, den, qsqr;
long *iN, *iS, *jE, *jW;
double *dN, *dS, *dW, *dE;
long r1, r2, c1, c2;
double cN, cS, cW, cE;
double *c, D;
double lambda;
long nthreads;



long _pMalloc;
long _lastRand;

void srand(long seed)
{
    _lastRand = seed;
}


long rand()
{
    _lastRand = _lastRand * 0xfef3f6f4f3f2f1;
    return _lastRand;
}

void *malloc_sr(long size)
{
    long p;

    // Align to the next page, so the memory allocated using 'malloc_sr' is always 'standalone'
    if((_pMalloc & 0xFFF) != 0)
    {
        _pMalloc = (_pMalloc & 0xFFFFFFFFFFFFF000) + 0x1000;
    }

    p = _pMalloc;

    _pMalloc += size;

    return (void *)p;
}



void random_matrix(double *I, long rows, long cols)
{
    long i, j;

    srand(7);

    for(i = 0 ; i < rows ; i = i + 1)
    {
        for(j = 0 ; j < cols ; j = j + 1)
        {
            I[i * cols + j] = (double)(rand()) /0xFFFFFFFFFFFFFFFF ;

        }
    }

}

double exp(double x)
{
    return 1;
}


long main()
{
    long i, j;

    _pMalloc = 0x500000000;

    niter = 10;
    rows = 1024;
    cols = 1024;
    r1   = 200; //y1 position of the speckle
    r2   = 500; //y2 position of the speckle
    c1   = 1000; //x1 position of the speckle
    c2   = 800; //x2 position of the speckle
    lambda = 0.5; //Lambda value
    niter = 100; //number of iterations


    size_I = cols * rows;
    size_R = (r2 - r1 + 1) * (c2 - c1 + 1);

    I = (double *)malloc_sr(size_I * 8);
    J = (double *)malloc_sr(size_I * 8);
    c  = (double *)malloc_sr(8 * size_I) ;

    iN = (long *)malloc_sr(8 * rows) ;
    iS = (long *)malloc_sr(8 * rows) ;
    jW = (long *)malloc_sr(8 * cols) ;
    jE = (long *)malloc_sr(8 * cols) ;


    dN = (double *)malloc_sr(8 * size_I) ;
    dS = (double *)malloc_sr(8 * size_I) ;
    dW = (double *)malloc_sr(8 * size_I) ;
    dE = (double *)malloc_sr(8 * size_I) ;


    for(i = 0; i < rows; i+=1)
    {
        iN[i] = i - 1;
        iS[i] = i + 1;
    }
    for(j = 0; j < cols; j+=1)
    {
        jW[j] = j - 1;
        jE[j] = j + 1;
    }
    iN[0]    = 0;
    iS[rows - 1] = rows - 1;
    jW[0]    = 0;
    jE[cols - 1] = cols - 1;


    random_matrix(I, rows, cols);

    for(k = 0;  k < size_I; k+=1)
    {
        J[k] = exp(I[k]) ;
    }



    for(iter = 0; iter < niter; iter+=1)
    {

        sum = 0;
        sum2 = 0;
        for(i = r1; i <= r2; i = i + 1)
        {
            for(j = c1; j <= c2; j = j + 1)
            {
                tmp   = J[i * cols + j];
                sum  += tmp ;
                sum2 += tmp * tmp;
            }
        }
        meanROI = sum / size_R;
        varROI  = (sum2 / size_R) - meanROI * meanROI;
        q0sqr   = varROI / (meanROI * meanROI);



//         #pragma omp parallel for shared(J, dN, dS, dW, dE, c, rows, cols, iN, iS, jW, jE) private(i, j, k, Jc, G2, L, num, den, qsqr)
        for(i = 0 ; i < rows ; i = i + 1)
        {
            for(j = 0; j < cols; j = j + 1)
            {

                k = i * cols + j;
                Jc = J[k];

                // directional derivates
                dN[k] = J[iN[i] * cols + j] - Jc;
                dS[k] = J[iS[i] * cols + j] - Jc;
                dW[k] = J[i * cols + jW[j]] - Jc;
                dE[k] = J[i * cols + jE[j]] - Jc;

                G2 = (dN[k] * dN[k] + dS[k] * dS[k] + dW[k] * dW[k] + dE[k] * dE[k]) / (Jc * Jc);

                L = (dN[k] + dS[k] + dW[k] + dE[k]) / Jc;

                num  = (0.5 * G2) - ((1.0 / 16.0) * (L * L)) ;
                den  = 1 + (0.25 * L);
                qsqr = num / (den * den);

                // diffusion coefficent (equ 33)
                den = (qsqr - q0sqr) / (q0sqr * (1 + q0sqr)) ;
                c[k] = 1.0 / (1.0 + den) ;

                // saturate diffusion coefficent
                if(c[k] < 0)
                {
                    c[k] = 0;
                }
                else if(c[k] > 1)
                {
                    c[k] = 1;
                }

            }

        }

//         #pragma omp parallel for shared(J, c, rows, cols, lambda) private(i, j, k, D, cS, cN, cW, cE)
        for(i = 0; i < rows; i = i + 1)
        {
            for(j = 0; j < cols; j = j + 1)
            {

                // current index
                k = i * cols + j;

                // diffusion coefficent
                cN = c[k];
                cS = c[iS[i] * cols + j];
                cW = c[k];
                cE = c[i * cols + jE[j]];

                // divergence (equ 58)
                D = cN * dN[k] + cS * dS[k] + cW * dW[k] + cE * dE[k];

                // image update (equ 61)
                J[k] = J[k] + 0.25 * lambda * D;

            }
        }
    }


    return 0;
}


