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
    /**
     * Solves the Linear Integer programming problem using the cutting plane algorithm.
     *
     * -Param mat: The matrix tableu that defines the constraints and the costs for the problem. The algorithm works with standard form Ax = b.
     */
    int solve(Matrix &mat, bool useBranchAndBound);
    int solve(std::vector<double> c, Matrix &a, std::vector<double> b);
    
private:
    /**
     * This function can be easly located in Fraction class. Since the cutting plane algorithm needs the calculation of the decimal, it is useful to have this function located in LIP class because the calculation of the decimal  is an important part of cutting plane algorithm.
     *
     * -Param val: The Fraction instance from where we want the decimal value
     *
     * -Returns the calculated decimal Fraction
     */
    Fraction calculateDecimal(Fraction val);
    
    void solveWithGomoryCuts(Matrix &mat, unsigned nonIntegerIndex);
    
    void solveWithBranchAndBound(Matrix &mat, unsigned nonIntegerIndex, unsigned currentNodeIndex);
};

