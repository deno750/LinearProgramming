//
//  LIP.cpp
//  LinearProgramming
//
//  Created by Denis Deronjic on 28/12/2019.
//  Copyright © 2019 Denis Deronjic. All rights reserved.
//

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
            double b = mat[i][0];
            if (!isInteger(b)) {
                nonIntegerIndex = i;
                break;
            }
        }
        if (nonIntegerIndex == 0) {
            optimal = true;
        }
        mat.increaseMatrix();
        //std::vector<double> gomoryCut;
        for (unsigned j = 0; j < mat.getColumnsCount(); ++j) {
            if (j == mat.getColumnsCount() - 1) {
                mat[mat.getRowsCount() - 1][j] = 1.0;
            } else {
                mat[mat.getRowsCount() - 1][j] = -calculateDecimal(mat[nonIntegerIndex][j]);
            }
            
            //gomoryCut.push_back();
        }
        
    }
    return 0;
}



