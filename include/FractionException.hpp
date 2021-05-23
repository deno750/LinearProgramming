//Source code taken from: https://github.com/talyssonoc/Fraction

#ifndef FRACTION_E_HPP
#define FRACTION_E_HPP

#include <stdexcept>
#include <string>

using namespace std;

class FractionException : public runtime_error {

public:
    FractionException (const string &err) : runtime_error(err) {}
};

#endif
