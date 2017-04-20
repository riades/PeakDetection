#include <cmath>
#include <ccomplex>
#include <vector>
#include <cstring>

#include "fftw3.h"

std::vector<double> mexh(size_t points, double a) {
  double A = 2 / (sqrt(3 * a) * pow(M_PI, 0.25));
  double wsq = a * a;

  std::vector<double> res(points);
  for (size_t i = 0; i < points; ++i) {
    double x = i - (points - 1.0) / 2;

    double xsq = x * x;
    double xwsq = xsq / wsq;
    double mod = (1 - xwsq);
    res[i] = A * mod * exp(- xwsq / 2);
  }

  return res;
}

unsigned long upper_power_of_two(unsigned long v) {
  v--;
  v |= v >> 1;
  v |= v >> 2;
  v |= v >> 4;
  v |= v >> 8;
  v |= v >> 16;
  v++;
  return v;
}

std::vector<std::vector<double>> cwt(const std::vector<double> &data,
                                     const std::vector<double> &widths) {
  std::vector<std::vector<double>> res(widths.size(),
                                       std::vector<double>(data.size()));

  // Determine the upper bound of widths. This would determine the FFT size
  size_t maxsz = 0;
  for (double width : widths)
    maxsz = std::max(maxsz, std::min(size_t(10 * width), data.size()));
  size_t n = upper_power_of_two(maxsz + data.size() - 1);
  size_t np = n / 2 + 1;

  // Allocate the buffers
  double *f = fftw_alloc_real(n);
  double *g = fftw_alloc_real(n);
  
  fftw_complex *F = fftw_alloc_complex(np);
  fftw_complex *G = fftw_alloc_complex(np);
  std::complex<double> *Fc = reinterpret_cast<std::complex<double>*>(F);
  std::complex<double> *Gc = reinterpret_cast<std::complex<double>*>(G);

  fftw_plan fp = fftw_plan_dft_r2c_1d(n, f, G, FFTW_ESTIMATE);
  fftw_plan bp = fftw_plan_dft_c2r_1d(n, F, g, FFTW_ESTIMATE);

  memset((double*)f, 0, sizeof(double) * n);
  memcpy((double*)f, data.data(), data.size() * sizeof(double));
  fftw_execute_dft_r2c(fp, f, F);

  for (size_t i = 0; i < widths.size(); ++i) {
    double width = widths[i];
    auto wavelet = mexh(std::min(size_t(10 * width), data.size()), width);

    memset((double*)g, 0, sizeof(double) * n);
    memcpy((double*)g, wavelet.data(), wavelet.size() * sizeof(double));

    fftw_execute_dft_r2c(fp, g, G);
  
    for (size_t i = 0; i < np; ++i)
      Gc[i] = Fc[i] * Gc[i];

    fftw_execute_dft_c2r(bp, G, g);
    for (size_t i = 0; i < n; ++i)
      g[i] /= n;

    size_t s1 = std::max(data.size(), wavelet.size());
    size_t start = (data.size() + wavelet.size() - 1 - s1) / 2;

    res[i].resize(s1, 0);
    memcpy(res[i].data(), (double*)g + start, s1 * sizeof(double));
  }

  fftw_free(f); fftw_free(g);
  fftw_free(F); fftw_free(G);
  fftw_destroy_plan(fp);
  fftw_destroy_plan(bp);
  
  return res;
}
