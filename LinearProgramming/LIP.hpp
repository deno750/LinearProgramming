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

#endif /* LIP_hpp */

//Includes functions for Linear Integer Programming
class LIP {
public:
    int solve(Matrix &mat);
    int solve(std::vector<double> c, Matrix &a, std::vector<double> b);
    
private:
    double epsilon = std::numeric_limits<double>::epsilon() * 100.0;
    bool isInteger(double val) {
        /*double n = calculateDecimal(val) + epsilon > 1 ? calculateDecimal(floor(val)) : calculateDecimal(val);
        //double nepsilon = n + epsilon;
        if (n <= epsilon) {
            return true;
        }
        return false;*/
        
        //return abs(n) <= 0.0001001;//std::numeric_limits<double>::epsilon();
        double decimal = calculateDecimal(val);
        return decimal == 0.0;
    }
    double calculateDecimal(double val) {
        double decimal = val - floor(val);
        decimal = decimal + epsilon < 1 ? decimal : 0.0; //Decimal becomes 1 when the value is 0.999999999
        return decimal <= epsilon ? 0.0 : decimal; //When the value is 0.000000000 something it's integer so is truncated
    }
};

