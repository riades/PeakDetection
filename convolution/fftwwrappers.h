#ifndef FFTWWRAPPERS_H_INCLUDED
#define FFTWWRAPPERS_H_INCLUDED

#include <vector>
#include <complex>
#include <fftw3.h>

class FFTWRealToComplexExecutor {
private:
    fftw_plan plan;
    
    const int inputSize;
    double* const inputBuffer;

    const int outputSize;
    fftw_complex* const outputBuffer;
public:
    FFTWRealToComplexExecutor(int len);
    ~FFTWRealToComplexExecutor();
    void setInput(const double* buffer, int size);
    void setInput(const std::vector<double> &vec);
    void execute();
    std::vector<fftw_complex > getOutput();
};

class FFTWComplexToRealExecutor {
private:
    fftw_plan plan;

    const int inputSize;
    fftw_complex* const inputBuffer;

    const int outputSize;
    double* const outputBuffer;
public:
    FFTWComplexToRealExecutor(int len);
    ~FFTWComplexToRealExecutor();
    void setInput(const fftw_complex* buffer, int size);
    void setInput(const std::vector<fftw_complex> &vec);
    void execute();
    std::vector<double> getOutput();
};

#endif

