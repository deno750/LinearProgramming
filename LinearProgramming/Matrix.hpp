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
    int getRowsCount() {return rows;}
    int getColumnsCount() {return columns;}
    Matrix(): rows(0), columns(0) {}
    Matrix(unsigned rows, unsigned columns) {
        this->rows = rows;
        this->columns = columns;
        matrix.resize(rows);
        for (int i = 0; i < rows; ++i) {
            matrix[i].resize(columns);
        }
    }
    Matrix(Matrix &m) {
        matrix = m.matrix;
        rows = m.rows;
        columns = m.columns;
    }
    Matrix(const std::vector<std::vector<double>> &m) {
        unsigned columnSize = 0;
        if(m.size() > 0) {
            columnSize = (unsigned) m[0].size();
            for (int i = 0; i<m.size(); ++i) {
                if (m[i].size() != columnSize) {
                    throw std::logic_error("The matrix must have all columns at the same size!!");
                }
            }
        }
        this->rows = (unsigned) m.size();
        this->columns = (unsigned) m[0].size();
        
        for (unsigned i = 0; i < this->rows; ++i) {
            std::vector<Fraction> row;
            for (unsigned j = 0; j < this->columns; ++j) {
                row.push_back(Fraction((float) m[i][j]));
            }
            this->matrix.push_back(row);
        }
        //this->matrix = m;
        
    }
    ~Matrix() {
        matrix.clear();
    }
    std::vector<Fraction>& operator [] (unsigned i) { return matrix[i]; }
    std::vector<Fraction> operator [] (unsigned i) const { return matrix[i]; }
    
    std::vector<unsigned> findIdentityMatrixIndices(unsigned startRowIndex, unsigned startColumndIndex);
    void visualize();
    //Increases matrix by 1. For example matrix (m,n) becomes a matrix (m+1, n+1)
    void increaseMatrix();
    
    
};

#endif /* Matrix_hpp */
