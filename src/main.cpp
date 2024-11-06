#include <af/array.h>
#include <af/defines.h>
#include <af/image.h>
#include <arrayfire.h>
#include <cstdio>
#include <math.h>

using namespace af;
const int S = 256;

float bell(float x, float m, float s){
  return exp( - (x-m)*(x-m)/s/s/2);
}

int main(int, char **) {
  try {
    static const int game_w = S, game_h = S;
    info();
    Window myWindow(S, S, "lenia");

    float b1, b2, s1, s2;

    const int T = 100;
    b1=0.12, b2=0.15;
    s1=0.12, s2=0.15;
    array U, A, G;
    const int R = 11;



    float aK[R*R] = {0};
    for(int i=0;i<R*R;i++) aK[i] = 1;
    aK[(R*R-1)/2] = 0;
    array uK(R, R, aK, afHost);
    float s = sum<float>(uK);


    const array K = uK/s;
    print("\nkern", K);

    A = af::randu(game_h, game_w, f32);
    while (!myWindow.close()) {
      myWindow.image(A);
      U = convolve(A, K);
      G = ((U>=b1)&(U<=b2)) - ((U<s1)||(U>s2));
      A = clamp(A + G * 1./T, 0, 1);
    }
  } catch (af::exception &e) {
    fprintf(stderr, "%s\n", e.what());
    throw;
  }
  return 0;
}
