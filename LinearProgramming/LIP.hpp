//
//  LIP.hpp
//  LinearProgramming
//
//  Created by Denis Deronjic on 28/12/2019.
//  Copyright © 2019 Denis Deronjic. All rights reserved.
//

#ifndef LIP_hpp
#define LIP_hpp

#include <stdio.h>
#include <math.h>
#include "Matrix.hpp"
#include "Fraction.hpp"

#endif /* LIP_hpp */

//Includes functions for Linear Integer Programming
class LIP {
public:
    int solve(Matrix &mat);
    int solve(std::vector<double> c, Matrix &a, std::vector<double> b);
    
private:
    Fraction calculateDecimal(Fraction val) {
        Fraction decimal = val - Fraction((int)floor(val.getDoubleValue()));
        return decimal;
    }
};

