#include <arrayfire.h>
#include <cstdio>
#define bell(x, m, s) exp( -((x)-(m))*((x)-(m))/(s)/(s)/2)

const int S = 256;
int main(int, char **) {
  try {
    af::info();
    std::puts("");
    af::Window win(S, S, "lenia");

    af::array U, A, G;
    float m, s;

    const int T = 10;
    const int R = 13;
    const int W = 2*R+1;
    m=0.150, s=0.015;

    float pK[W*W] = {0};
    for(int i=0;i<W;i++){
      for(int j=0;j<W;j++) {
        pK[j*W+i] = sqrt((i-R)*(i-R)+(j-R)*(j-R))/R;
      }
    }

    af::array uK(W, W, pK, afHost);
    uK = (uK<1)*bell(uK, 0.7, 0.15);

    A = af::randu(S, S, f32);

    const af::array K = uK/af::sum<float>(uK);

    while (!win.close()) {
      win.image(A);
      U = convolve(A, K);
      G = bell(U, m, s)*2 - 1;
      A = clamp(A + G * 1./T, 0, 1);
    }
  } catch (af::exception &e) {
    std::fprintf(stderr, "%s\n", e.what());
    throw;
  }
  return 0;
}
