#include "ModelParser.hpp"
#include <fstream>
#include <string>
#include <iostream>
#include <unordered_map>
#include <vector>
#include <regex>

using namespace std;

inline bool isInteger(const std::string & s)
{
   if(s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) return false;

   char * p;
   strtol(s.c_str(), &p, 10);

   return (*p == 0);
}

void tokenize(std::string const &str, const char delim,
            std::vector<std::string> &out)
{
    size_t start;
    size_t end = 0;
 
    while ((start = str.find_first_not_of(delim, end)) != std::string::npos)
    {
        end = str.find(delim, start);
        out.push_back(str.substr(start, end - start));
    }
}

Matrix parseFile(std::string filePath) {
    std::fstream reader(filePath);
    if (!reader) {
        cout << "Error with input file" << endl;
    }
    bool minimize = true;
    std::string token;

    while (!reader.eof()) {
        std::getline(reader, token);
        if (token.find("Minimize") != string::npos) {
            break;
        } else if (token.find("Maximize") != string::npos) {
            minimize = false;
            break;
        }
    }

    // Parsing objective function
    std::unordered_map<std::string, int> variables; // Maps the variable name to tableu index
    bool positive = true;
    int number = 1;
    int index = 1;

    std::vector<int> obj;
    Matrix matrix;
    matrix.increaseMatrix();

    while (!reader.eof()) {
        std::getline(reader, token);
        std::regex regex{R"([\s\n\r]+)"}; // split on space and comma
        std::sregex_token_iterator it{token.begin(), token.end(), regex, -1};
        std::vector<std::string> words{it, {}};
        if (token.find("Subject To") != string::npos) {
            break;
        } 
        for (std::string v : words) {
            if (v == "") {
                continue;
            }
            //cout << v << endl;
        
            if (v == "-") {
                positive = false;
                continue;
            }

            if (v == "+") {
                positive = true;
                continue;
            }

            if (isInteger(v)) {
                number = atoi(v.c_str());
            } else {
                // Variable name
                variables[v] = index;
                
                matrix.addColumns(1);
                number = positive ? number : -number;
                if (!minimize) {
                    number = -number;
                }
                matrix[0][index] = number;
                index++;

                number = 1;
            }
        }
        
    }

    //matrix.visualize();

    int raw_index = 1;
    positive = true;
    // Parsing constraints
    while (!reader.eof()) {
        std::getline(reader, token);
        //cout << token << endl;
        
        std::regex regex{R"([\s\n\r]+)"}; // split on space and comma
        std::sregex_token_iterator it{token.begin(), token.end(), regex, -1};
        std::vector<std::string> words{it, {}};
        if (token.find("Bounds") != string::npos) {
            break;
        }
        matrix.addRows(1);

        bool addingRhs = false;
        bool greaterEq = false;
        for (int i = 0; i < words.size() ; i++) {
            std::string v = words[i];
            if (v == "") {
                continue;
            }
            //cout << v << endl;
        
            if (v == "-") {
                positive = false;
                continue;
            }

            if (v == "+") {
                positive = true;
                continue;
            }
            if (v == "=" || v == "<=" || v == ">=") {
                addingRhs = true;
                if (v == "<=") {
                    matrix.addColumns(1);
                    matrix[raw_index][matrix.getColumnsCount() - 1] = Fraction::ONE;
                } else if (v == ">=") {
                    greaterEq = true;
                    for (int i = 0; i < matrix.getColumnsCount(); i++) {
                        //cout << matrix[raw_index][i] << endl;
                        matrix[raw_index][i] = Fraction((int)-1) * matrix[raw_index][i];
                        ///cout << matrix[raw_index][i] << endl;
                    }
                    matrix.addColumns(1);
                    matrix[raw_index][matrix.getColumnsCount() - 1] = Fraction::ONE;
                    
                }
                continue;
            }

            if (isInteger(v)) {
                number = atoi(v.c_str());
                
                if (addingRhs) {
                    if (!positive) {
                        number = -number;
                    }
                    if (greaterEq) {
                        number = -number;
                        greaterEq = false;
                    }
                    matrix[raw_index][0] = number;
                    if (number < 0) {
                        for (int i = 0; i < matrix.getColumnsCount(); i++) {
                            //cout << matrix[raw_index][i] << endl;
                            matrix[raw_index][i] = Fraction((int)-1) * matrix[raw_index][i];
                            //cout << matrix[raw_index][i] << endl;
                        }
                    }
                    raw_index++; // When we are adding rhs it means that the parsing of the row is finished, so we increment the raw index
                
                    // Reinit to defaults
                    positive = true;
                    number = 1;
                }
            } else {
                // Variable name
                int col_idx = variables[v];
                
                
                matrix[raw_index][col_idx] = positive ? number : -number;

                // Reinit to defaults
                positive = true;
                number = 1;
            }
        }
        addingRhs = false;
    }
    matrix.updateBasis(); // Check whetere there is a basis in matrix
    matrix.visualize();
    return matrix;
}