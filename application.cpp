#include "BigInteger.h"
#include <iostream>
using namespace std;

int main() {
    for (int n; cin >> n;) {
        BigInteger fac = 1;
        for (int i = 1; i <= n; ++i)
            fac *= i;
        cout << fac << endl;
    }
    return 0;
}

