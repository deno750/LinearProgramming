#include "ModelParser.hpp"
#include <fstream>
#include <string>
#include <iostream>
#include <unordered_map>
#include <vector>
#include <regex>
#include <cstdlib>

using namespace std;


bool is_number(const std::string& s)
{
    char* end = nullptr;
    double val = strtod(s.c_str(), &end);
    return end != s.c_str() && *end == '\0' && val != HUGE_VAL;
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
    float number = 1;
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

            if (is_number(v)) {
                //number = atoi(v.c_str());
                number = std::stod(v);
            } else {
                // Variable name
                variables[v] = index;
                
                matrix.addColumns(1);
                number = positive ? number : -number;
                if (!minimize) {
                    number = -number;
                }
                matrix[0][index] = Fraction(number);
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
                        matrix[raw_index][i] =  matrix[raw_index][i] * -1;
                        ///cout << matrix[raw_index][i] << endl;
                    }
                    matrix.addColumns(1);
                    matrix[raw_index][matrix.getColumnsCount() - 1] = Fraction::ONE;
                    
                }
                continue;
            }

            if (is_number(v)) {
                //number = atoi(v.c_str());
                number = std::stod(v);
                
                if (addingRhs) {
                    if (!positive) {
                        number = -number;
                    }
                    if (greaterEq) {
                        number = -number;
                        greaterEq = false;
                    }
                    matrix[raw_index][0] = Fraction(number);
                    if (number < 0) {
                        for (int i = 0; i < matrix.getColumnsCount(); i++) {
                            //cout << matrix[raw_index][i] << endl;
                            matrix[raw_index][i] = matrix[raw_index][i] * -1;
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
                
                
                matrix[raw_index][col_idx] = Fraction(positive ? number : -number);

                // Reinit to defaults
                positive = true;
                number = 1;
            }
        }
        addingRhs = false;
    }
    matrix.updateBasis(); // Check whetere there is a basis in matrix
    //matrix.visualize();
    return matrix;
}

Matrix parseFile2(std::string filePath) {

    std::fstream reader(filePath);
    if (!reader) {
        cout << "Error with input file" << endl;
    }
    std::string in;

    std::regex type_pattern(R"([\s\t]*((Minimize)|(Maximize)|(Min)|(Max))[\s\t]*)", std::regex_constants::icase);
    std::regex obj_pattern(R"(([\s\t]*[a-zA-Z0-9]*[\s\t]*:)?[\s\t]*[\+\-]?[\s\t]*\d*[\s\t]*[a-zA-Z][a-zA-Z0-9]*[\s\t]*([\+\-][\s\t]*\d*[\s\t]*[a-zA-Z][a-zA-Z0-9]*[\s\t]*)*)", std::regex_constants::icase);

    bool minimize = true;
    auto i = 0;
    while (std::getline(reader, in)) {
        //cout << in << endl;
        if (regex_match (in, type_pattern )) {
            cout << "string:literal => matched\n";
        }
        std::smatch m;
        if (regex_match (in, m, obj_pattern )) {
            cout << "objective => matched\n";
            std::sregex_token_iterator it{in.begin(), in.end(), obj_pattern, 2};
            std::vector<std::string> words{it, {}};
            for (auto str : words) {
                cout << "Hi: " << str << endl;
            }
        }
            
        if (i++ == 10) {
            break;
        }
    }
    



    Matrix mat;
    return mat;
}