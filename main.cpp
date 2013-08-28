#include "BigInteger.h"
#include <iostream>
using namespace std;

char str[65535] = "123456";

int main() {
    BigInteger x("5"), y(5), z(str);
    cout << BigInteger::ZERO.pow(3) << endl;
    cout << x.pow(3) << endl;

    for (BigInteger n; cin >> n;) {
        BigInteger fac = 1;
        for (BigInteger i = 1; i <= n; ++i)
            fac *= i;
        cout << fac << endl;
    }
    return 0;
}
