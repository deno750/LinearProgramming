//
//  Matrix.hpp
//  LinearProgramming
//
//  Created by Denis Deronjic on 16/11/2019.
//  Copyright © 2019 Denis Deronjic. All rights reserved.
//

#ifndef Matrix_hpp
#define Matrix_hpp

#include <stdio.h>
#include <vector>
#include "Fraction.hpp"

class Matrix {
private:
    unsigned rows;
    unsigned columns;
    std::vector<std::vector<Fraction>> matrix;
    
public:
    //Constructors
    Matrix();
    Matrix(unsigned rows, unsigned columns);
    Matrix(Matrix &m);
    Matrix(const std::vector<std::vector<double>> &m);
    
    //Destructor
    ~Matrix();
    
    //Operator Override
    std::vector<Fraction>& operator [] (unsigned i);
    std::vector<Fraction> operator [] (unsigned i) const;
    
    std::vector<unsigned> findIdentityMatrixIndices(unsigned startRowIndex, unsigned startColumndIndex);
    int getRowsCount();
    int getColumnsCount();
    void visualize();
    //Increases matrix by 1. For example matrix (m,n) becomes a matrix (m+1, n+1)
    void increaseMatrix();
    
    
};

#endif /* Matrix_hpp */
