// modulus implementation
int mod(int a, int b) {
   int res = a % b;
   if (res < 0)
      res += b;
   return res;
}


int square(int x) {
  return x*x;
}


int pow(int base, int exp)
{
    int result = 1;
    while (exp)
    {
        if (exp & 1)
            result *= base;
        exp >>= 1;
        base *= base;
    }

    return result;
}
