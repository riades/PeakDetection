#include "fftwconvolve.h"

std::vector<fftw_complex> multiply(const std::vector<fftw_complex> &a, 
                                   const std::vector<fftw_complex> &b) {
    fftw_complex* buffer(fftw_alloc_complex(a.size()));
    std::vector<fftw_complex> result(buffer, buffer + a.size());
    for (int i = 0; i < result.size(); i++) {
        result[i][0] = a[i][0]*b[i][0] - a[i][1]*b[i][1];
        result[i][1] = a[i][1]*b[i][0] + a[i][0]*b[i][1];
    }
    return result;
}

std::vector<double> fftwConvolve(const std::vector<double> &a, const std::vector<double> &b) {
    size_t length = a.size() + b.size() - 1;
    FFTWRealToComplexExecutor r2c(length);
    r2c.setInput(a);
    r2c.execute();
    std::vector<fftw_complex> fftA = r2c.getOutput();
    r2c.setInput(b);
    r2c.execute();
    std::vector<fftw_complex> fftB = r2c.getOutput();

    FFTWComplexToRealExecutor c2r(length);
    c2r.setInput(multiply(fftA, fftB));
    c2r.execute();
    std::vector<double> ifftC = c2r.getOutput();

    for (size_t i = 0; i < ifftC.size(); i++) {
        ifftC[i] /= length;
    }

    size_t lengthOut = std::max(a.size(), b.size());
    size_t start = (length - lengthOut) / 2;
    ifftC.resize(lengthOut, 0);
    return std::vector<double>(ifftC.begin() + start, ifftC.end());
}