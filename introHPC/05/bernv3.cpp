#include <iostream>
#include <gmp.h>

auto main() -> int
{
    mpz_class num1, num2, num3;
    num1 = 5;
    num2 = 10;
    num3 = num1 + num2;
    cout << num3 << endl;
    return 0;
}
