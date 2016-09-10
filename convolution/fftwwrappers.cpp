#include <cstring>

#include "fftwwrappers.h"

FFTWRealToComplexExecutor::FFTWRealToComplexExecutor(int len) :
    inputSize(len),
    inputBuffer(fftw_alloc_real(len)),
    outputSize(len/2 + 1),
    outputBuffer(fftw_alloc_complex(len/2 + 1)) {
    plan = fftw_plan_dft_r2c_1d(len, inputBuffer, outputBuffer, FFTW_ESTIMATE);
}

FFTWRealToComplexExecutor::~FFTWRealToComplexExecutor() {
    fftw_destroy_plan(plan);
    fftw_free(inputBuffer);
    fftw_free(outputBuffer);
}

void FFTWRealToComplexExecutor::setInput(const double* buffer, int size) {
    memcpy(inputBuffer, buffer, sizeof(double)*size);
    memset(&inputBuffer[size], 0, sizeof(double)*(inputSize - size));
}

void FFTWRealToComplexExecutor::setInput(const std::vector<double> &vec) {
    setInput(&vec[0], vec.size());
}

void FFTWRealToComplexExecutor::execute() {
    fftw_execute_dft_r2c(plan, inputBuffer, outputBuffer);
}

std::vector<fftw_complex> FFTWRealToComplexExecutor::getOutput() {
    return std::vector<fftw_complex>(outputBuffer, outputBuffer + outputSize);
}

FFTWComplexToRealExecutor::FFTWComplexToRealExecutor(int len) : 
    inputSize(len/2 + 1),
    inputBuffer(fftw_alloc_complex(len/2 + 1)),
    outputSize(len),
    outputBuffer(fftw_alloc_real(len)) {
    plan = fftw_plan_dft_c2r_1d(len, inputBuffer, outputBuffer, FFTW_ESTIMATE);
}

FFTWComplexToRealExecutor::~FFTWComplexToRealExecutor() {
    fftw_destroy_plan(plan);
    fftw_free(inputBuffer);
    fftw_free(outputBuffer);
}

void FFTWComplexToRealExecutor::setInput(const fftw_complex* buffer, int size) {
    memcpy(inputBuffer, buffer, sizeof(fftw_complex)*size);
    memset(&inputBuffer[size], 0, sizeof(fftw_complex)*(inputSize - size));
}

void FFTWComplexToRealExecutor::setInput(const std::vector<fftw_complex> &vec) {
    setInput(&vec[0], vec.size());
}

void FFTWComplexToRealExecutor::execute() {
    fftw_execute_dft_c2r(plan, inputBuffer, outputBuffer);
}

std::vector<double> FFTWComplexToRealExecutor::getOutput() {
    return std::vector<double>(outputBuffer, outputBuffer + outputSize);
}
