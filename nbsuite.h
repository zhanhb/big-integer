/* 
 * File:   nbsuite.h
 * Author: zhanhb
 *
 * Created on August 11, 2013, 11:12 PM
 */

#ifndef NBSUITE_H
#define	NBSUITE_H

#include <string>
#include <ctime>
#include <iostream>
#include <exception>
#include <libgen.h>
#include <typeinfo>

class _C_simple_test_suite {
    std::string _M_name;
    time_t _L_start;

    static double formatTime(time_t t) {
        return (t + 0.0) / CLOCKS_PER_SEC;
    }

    void init() {
        _M_name = basename(&_M_name[0]);
        for (int i = _M_name.length(); i--;) {
            if (_M_name[i] == '.') {
                _M_name = _M_name.substr(0, i);
                break;
            }
        }
        std::cout << "%SUITE_STARTING% " << _M_name << std::endl;
        std::cout << "%SUITE_STARTED%" << std::endl;
    }
public:

    class _C_test_exception {
        int _M_line;
        std::string _M_message, _M_file;
    public:

        _C_test_exception(const std::string &message, const std::string &file, int line)
        : _M_line(line), _M_message(message), _M_file(file) { }

        template<typename _CharT, typename _Traits>
        friend std::basic_ostream<_CharT, _Traits> & operator<<(std::basic_ostream<_CharT, _Traits> &os, _C_test_exception &ex) {
            return os << ex._M_message << ", " << ex._M_file << ", " << ex._M_line;
        }
    };

    typedef void (*_M_method)();

    _C_simple_test_suite(const std::string &name) : _M_name(name), _L_start(clock()) {
        init();
    }

    _C_simple_test_suite() : _M_name(), _L_start(clock()) {
        init();
    }

    void _M_test_method(_M_method method, const std::string & methodName) {
        std::cout << "%TEST_STARTED% " << methodName
                << " (" << _M_name << ")" << std::endl;
        time_t st = clock();
        try {
            method();
        } catch (_C_test_exception & ex) {
            std::cout << "%TEST_FAILED% time=" << formatTime(clock() - st)
                    << " testname=" << methodName
                    << " (" << _M_name << ")"
                    << " message=" << ex << std::endl;
        } catch (std::exception & ex) {
            std::cout << "%TEST_FAILED% time=" << formatTime(clock() - st)
                    << " testname=" << methodName
                    << " (" << _M_name << ")"
                    << " message=" << typeid(ex).name() << " " << ex.what() << std::endl;
        }
        std::cout << "%TEST_FINISHED% time=" << formatTime(clock() - st) << " "
                << methodName << " (" << _M_name << ")" << std::endl;
    }

    ~_C_simple_test_suite() {
        std::cout << "%SUITE_FINISHED% time=" << formatTime(clock() - _L_start) << std::endl;
    }
};
#define TestSuiteAssert(x)  do {                                                    \
    if(!(x)) {                                                                      \
        throw _C_simple_test_suite::_C_test_exception(#x, __FILE__, __LINE__);      \
    }                                                                               \
} while(false)

#define TestStart(...) do { _C_simple_test_suite theSuite(__FILE__)
#define TestMethod(methodName) do { theSuite._M_test_method(methodName, #methodName);} while(false)
#define TestEnd() } while(false)
#include <cassert>
#define RC_INVOKED 
#undef assert
#define assert TestSuiteAssert

#endif	/* NBSUITE_H */

