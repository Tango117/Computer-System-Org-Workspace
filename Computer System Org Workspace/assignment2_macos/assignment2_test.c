
#include <stdio.h>
#include <stdlib.h>
#define FOUR_BIT_MASK 0xF

void print_hex(void *p)
{
  // copy the value that p points to into an unsigned integer variable.
  unsigned int x = *((unsigned int *) p);

  if(x == 0){
    printf("0");
    return;
  }
  char hex[8];
  int i = 0;

  while(x > 0){
    unsigned int y = x & FOUR_BIT_MASK;
    if(y < 10){
      hex[i] = y + 48;
    }else{
      hex[i] = y + 87;
    }
    x = x >> 4;
    i += 1;
  }

  for(int j = i - 1; j >= 0; j--){
    printf("%c", hex[j]);
  }
  printf("\n");

}

int divide(int x, int y) {

  if (y == 0) {
    printf("Error: Divisor is by 0\n");
    exit(1);       // this causes the program to terminate
  }

  int sign = 1;

  if(x < 0){
    x *= -1;
    sign *= -1;
  }
  if(y < 0){
    y *= -1;
    sign *= -1;
  }

  unsigned long x1 = (*((unsigned long *) &x)) & 0xFFFFFFFF;
  unsigned long y1 = (*((unsigned long *) &y)) & 0xFFFFFFFF;

  y1 = y1 << 32;

  unsigned int quotient = 0;

  for(int i = 0; i < 32; i++){
    y1 = y1 >> 1;
    quotient = quotient << 1;
    if(y1 <= x1){
      quotient = quotient | 1;
      x1 = x1 - y1;
    }
  }

  int signed_quotient = quotient;
  signed_quotient *= sign;

  return signed_quotient;
}

// select bit 31, shifted all the way to the right.

#define SIGN(x) (((x) & 0x80000000) >> 31)

// select bits 23 through 30, shifted right by 23 bits

#define EXP(x) (((x) & 0x7F800000) >> 23)

// select bits 0 through 22 (the rightmost 23 bits)

#define FRAC(x) ((x) & 0x7FFFFF)

float float_add(float f, float g){

  // We need to treat the values stored in f and g
  // as 32-bit unsigned numbers. See the hint sheet
  // for ways to do that.  We then need to extract the
  // sign, exponent, and fraction fields from f
  // and g, using the SIGN, EXP, and FRAC macros
  // above.

  unsigned int *f1 = (unsigned int*) &f;
  unsigned int *g1 = (unsigned int*) &g;

  printf("%u and %u\n", *f1, *g1);

  unsigned int sign_f = SIGN(*f1);
  unsigned int sign_g = SIGN(*g1);

  unsigned int exp_f = EXP(*f1);
  unsigned int exp_g = EXP(*g1);

  unsigned int frac_f = FRAC(*f1);
  unsigned int frac_g = FRAC(*g1);

  printf("%u and %u and %u\n", sign_f, exp_f, frac_f);
  printf("%u and %u and %u\n", sign_g, exp_g, frac_g);

  if(f == 0){
    return g;
  }
  if(g == 0){
    return f;
  }

  unsigned int mantissa_f = frac_f | 0x800000;
  unsigned int mantissa_g = frac_g | 0x800000;

  // printf("Mantissa: %u and %u\n", mantissa_f, mantissa_g);

  if(exp_f < exp_g){
    mantissa_f = mantissa_f >> (exp_g - exp_f);
    exp_f = exp_g;
  }else{
    mantissa_g = mantissa_g >> (exp_f - exp_g);
    exp_g = exp_f;
  }

  unsigned int sign_res;
  unsigned int mantissa_res;
  unsigned int exp_res = exp_f;

  if (sign_f == sign_g) {

    sign_res = sign_f;
    mantissa_res = mantissa_f + mantissa_g;
    if(mantissa_res > 0x1000000){
      mantissa_res = mantissa_res >> 1;
      exp_res += 1;
    }

  }
  else{
    printf("Mantissa: %u and %u\n", mantissa_f, mantissa_g);
    if(mantissa_f > mantissa_g){
      sign_res = sign_f;
      mantissa_res = mantissa_f - mantissa_g;
    }else if(mantissa_g > mantissa_f){
      sign_res = sign_g;
      mantissa_res = mantissa_g - mantissa_f;
    }else{
      return 0.0;
    }

    while((mantissa_res & 0x800000) == 0){
      printf("%u Shifted Left\n", mantissa_res);
      mantissa_res = mantissa_res << 1;
      exp_res -= 1;
    }
  }

  printf("Result: %u and %u and %u\n", sign_res, exp_res, mantissa_res);
  unsigned int result = (sign_res << 31) | ((exp_res & 0xFF) << 23) | (mantissa_res & 0x7FFFFF);

  return *((float *) &result);
}


int main(){
  // int x = 0;
  // printf("%x\n", x);
  // printf("Enter a number to print in hex > ");
  // scanf("%d", &x);
  // print_hex(&x);
  // divide(1823, 3);
  float x = float_add(54.5, -294.25);
  // float x = EXP(32);
  printf("%f\n", x);

}
