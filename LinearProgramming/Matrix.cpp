//
//  Matrix.cpp
//  LinearProgramming
//
//  Created by Denis Deronjic on 16/11/2019.
//  Copyright © 2019 Denis Deronjic. All rights reserved.
//

#include <iostream>
#include <iomanip>
#include <math.h>

#include "Fraction.hpp"
#include "Matrix.hpp"

Matrix::Matrix(): rows(0), columns(0) {}

Matrix::Matrix(unsigned rows, unsigned columns) {
    this->rows = rows;
    this->columns = columns;
    matrix.resize(rows);
    for (int i = 0; i < rows; ++i) {
        matrix[i].resize(columns);
    }
    basisIndexes = findBasis();
}

Matrix::Matrix(Matrix &m) {
    matrix = m.matrix;
    rows = m.rows;
    columns = m.columns;
    basisIndexes = findBasis();
}

Matrix::Matrix(const std::vector<std::vector<double>> &m) {
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
    basisIndexes = findBasis();
}

Matrix::~Matrix() {
    matrix.clear();
}

int Matrix::getRowsCount() {return rows;}
int Matrix::getColumnsCount() {return columns;}

std::unordered_map<unsigned, unsigned> Matrix::findBasis() {
    std::unordered_map<unsigned, unsigned> map;
    for (unsigned j = 0; j < this->columns; ++j) {
        unsigned count1 = 0;
        unsigned rowIndex = 0;
        if (matrix[0][j] == 0.0) {
            for (unsigned k = 1; k < this->rows; ++k) {
                if (matrix[k][j] == 1.0) {
                    count1++;
                    rowIndex = k;
                } else if (matrix[k][j] == 0.0){
                    //Do nothing it's ok
                } else {
                    break;
                }
            }
        } else {
            continue;
        }
        if (count1 == 1) {
            if (map[rowIndex] == 0) {
                map[rowIndex] = j;
            }
        }
        
    }
    return map;
}

void Matrix::visualize() {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j< columns; ++j) {
            if(matrix[i][j] == -0) {
                matrix[i][j] = 0;
            }
            std::cout << std::setw(11) << matrix[i][j].toString();
            if (j == 0) {
                std::cout << "   | ";
            }
        }
        if (i == 0) {
            std::cout << std::endl;
        }
        
        std::cout << std::endl;
    }
}

void Matrix::increaseMatrix() {
    
    for (unsigned i = 0; i < rows; ++i) { //Adding new column
        matrix[i].push_back(Fraction::ZERO);
        for (unsigned k = 0; k < columns + 1; ++k) {
            std::cout << matrix[i][k].toString() << ",";
        }
        std::cout << std::endl;
    }
    columns += 1;
    std::vector<Fraction> vec(columns, Fraction::ZERO);
    matrix.push_back(vec); //Adding new row
    rows += 1;
}

void Matrix::addColumns(unsigned numberOfColumns) {
    for (unsigned i = 0; i < rows; ++i) {
        for (unsigned j = 0; j < numberOfColumns; ++j) {
            matrix[i].push_back(Fraction::ZERO);
        }
    }
    columns += numberOfColumns;
}

void Matrix::removeColumns(unsigned numberOfColumns) {
    for (unsigned i = 0; i < rows; ++i) {
        for (unsigned j = columns - 1; j > columns - numberOfColumns; --j) {
            matrix[i].pop_back();
        }
    }
    columns -= numberOfColumns;
}

void Matrix::insertInBasis(unsigned rowIndex, unsigned indexOfVectorToInsert) {
    basisIndexes[rowIndex] = indexOfVectorToInsert;
}

unsigned Matrix::getBasisIndexAtRow(unsigned rowIndex) {
    return basisIndexes[rowIndex];
}

unsigned Matrix::basisSize() {
    return basisIndexes.size();
}

std::vector<Fraction>& Matrix::operator [] (unsigned i) { return matrix[i]; }
std::vector<Fraction> Matrix::operator [] (unsigned i) const { return matrix[i]; }
