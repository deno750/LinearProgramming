//
//  Matrix.hpp
//  LinearProgramming
//
//  Created by Denis Deronjic on 16/11/2019.
//  Copyright © 2019 Denis Deronjic. All rights reserved.
//

#ifndef Matrix_HPP
#define Matrix_HPP

#include <stdio.h>
#include <vector>
#include <unordered_map>
#include "Fraction.hpp"

class Matrix {
private:
    unsigned rows;
    unsigned columns;
    std::vector<std::vector<Fraction>> matrix;
    std::unordered_map<unsigned, unsigned> basisIndexes; //The key is the row index and the value is the column index. Example {1: 3} indicates that in the 1st row the basis vector is x3 where 3 is the index of the vector in basis.
    
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
    
    std::unordered_map<unsigned, unsigned> findBasis();
    void updateBasis();
    int getRowsCount();
    int getColumnsCount();
    void visualize();
    //Increases matrix by 1. For example matrix (m,n) becomes a matrix (m+1, n+1)
    void increaseMatrix();
    void addColumns(unsigned numberOfColumns);
    void addRows(unsigned numberOfRows);
    void removeColumns(unsigned numberOfColumns);
    void insertInBasis(unsigned rowIndex, unsigned indexOfVectorToInsert);
    unsigned getBasisIndexAtRow(unsigned rowIndex);
    unsigned basisSize();
    
};

#endif /* Matrix_hpp */
