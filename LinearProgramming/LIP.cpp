//
//  LIP.cpp
//  LinearProgramming
//
//  Created by Denis Deronjic on 28/12/2019.
//  Copyright © 2019 Denis Deronjic. All rights reserved.
//

#include <iostream>
#include "LIP.hpp"
#include "Simplex.hpp"

int LIP::solve(std::vector<double> c, Matrix &a, std::vector<double> b) {
    return 0;
}

int LIP::solve(Matrix &mat) {
    
    bool optimal = false; //Integer optimal solution
    Simplex simplex;
    while (!optimal) {
        int res = simplex.solve(mat);
        if (res == -1) { //The problem is impossible!
            return -1;
        }
        //Find if there's one solution that is not integer
        unsigned nonIntegerIndex = 0;
        for (unsigned i = 1; i < mat.getRowsCount(); ++i) {
            Fraction b = mat[i][0];
            if (!b.isInteger()) {
                nonIntegerIndex = i;
                break;
            }
        }
        if (nonIntegerIndex == 0) {
            optimal = true;
            return 0;
        }
        
        std::cout << "The optimal solution is not integer." << std::endl;
        solveWithGomoryCuts(mat, nonIntegerIndex);
        
    }
    return 0;
}

Fraction LIP::calculateDecimal(Fraction val) {
    Fraction decimal = val - Fraction((int)floor(val.getDoubleValue()));
    return decimal;
}

void LIP::solveWithGomoryCuts(Matrix &mat, unsigned nonIntegerIndex) {
    mat.increaseMatrix();
    //Solving with gomory cuts
    for (unsigned j = 0; j < mat.getColumnsCount(); ++j) {
        if (j == mat.getColumnsCount() - 1) {
            mat[mat.getRowsCount() - 1][j] = Fraction::ONE;
            mat.insertInBasis(mat.getRowsCount() - 1, j);
        } else {
            Fraction decimal = calculateDecimal(mat[nonIntegerIndex][j]) * -1;
            mat[mat.getRowsCount() - 1][j] = decimal;
        }
    }
    
    std::cout << "Added gomory cut\n" << std::endl;
    mat.visualize();
}

void LIP::solveWithBranchAndBound(Matrix &mat, unsigned nonIntegerIndex) {
    
}



