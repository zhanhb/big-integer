/* 
 * File:   suite.h
 * Author: zhanhb
 *
 * Created on August 11, 2013, 12:12 PM
 */

#ifndef SUITE_H
#define	SUITE_H

#ifndef _MSC_VER
#include "nbsuite.h"
#else
#include <ctime>
#define TestStart(...)
#define TestMethod(method) do {\
    time_t st = clock();\
    std::cout << #method << " " << "TEST_STARTED" << std::endl;\
    method();\
    std::cout << #method << " " << "TEST_FINISHED time=" << (clock() - st + 0.0) / CLOCKS_PER_SEC\
                 << std::endl;\
} while(false)
#define TestEnd()
#endif

#endif	/* SUITE_H */
