/* 
 * File:   testRightShift.cpp
 * Author: zhanhb
 *
 * Created on Aug 11, 2013, 10:18:16 PM
 */

#define _BIG_INTEGER_DEBUG

#include "includes.h"
#include "BigInteger.h"
#include "suite.h"
using namespace std;

void bug_1() {
    BigInteger x("1124000727777607680000");
    assert(x >> 18 == "4287722502813750"); //262144
}

int main(int argc, char** argv) {
    cout << __FILE__ << endl;
    TestStart();
    TestMethod(bug_1);
    TestEnd();
    return (EXIT_SUCCESS);
}

