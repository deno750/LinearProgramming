//
//  Matrix.cpp
//  LinearProgramming
//
//  Created by Denis Deronjic on 16/11/2019.
//  Copyright © 2019 Denis Deronjic. All rights reserved.
//

#include "Matrix.hpp"
#include <iostream>
#include <iomanip>
#include <math.h>
#include "Fraction.hpp"

std::vector<unsigned> Matrix::findIdentityMatrixIndices(unsigned startRowIndex = 0, unsigned startColumndIndex = 0) {
    std::vector<unsigned> indexes;
    unsigned i = 0; //Row index
    for (unsigned j = startColumndIndex; j < this->columns; ++j) {
        unsigned count1 = 0;
        if (matrix[i][j] == 0.0) {
            for (unsigned k = i + 1; k < this->rows; ++k) {
                if (matrix[k][j] == 1.0) {
                    count1++;
                } else if (matrix[k][j] == 0.0){
                    //Do nothing it's ok
                } else {
                    break;
                }
            }
        } else if (matrix[i][j] == 1.0) {
            count1++;
        } else {
            continue;
        }
        if (count1 == 1) {
            indexes.push_back(j);
        }
        
    }
    return indexes;
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
    
    for (unsigned i = 0; i < rows; ++i) {
        matrix[i].push_back(Fraction(0, 1));
    }
    columns += 1;
    std::vector<Fraction> vec(columns, Fraction(0, 1));
    matrix.push_back(vec);
    rows += 1;
}
