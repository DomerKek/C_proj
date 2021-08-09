//Vladyslav Postrelko, K-13;
#ifndef LAB3_3_BUILDER_H
#define LAB3_3_BUILDER_H

#include "Info.h"
#include "Lexer.h"
#include <fstream>
#include <map>

typedef unsigned long int ulint;

class BuilderError:public std::exception{
private:
    std::string message;
    const char* smal_message;
    static const std::map<int, const char*> explanation;
    const char* error_explanation(int error_number);
    int error_number, number_of_line;
public:
    const char* what();
    BuilderError(int error_number, int number_of_line);
    BuilderError(int error_number, int number_of_line, const char* msg);
};

class Builder {//performs data loading in the Info
public:
    void loadData(Info& inf, const char* filename);//opens the file and loads the data
private:
    void processing(); //processing file line
    void do_header(); //processing header
    void do_footer(); //processing footer
    void do_line();   //processing line
    std::ifstream* pfilestream;
    Info* info;
    ulint strtouint(std::string &str); //convert string to unsigned long int
    ulint max_prcnt, min_prcnt, number;
    bool is_header, is_footer, is_line, is_max_prcnt, is_min_prcnt;
    std::string tmp, field;
    std::string fields[9];
    LineType type_of_line;
    int number_of_line;
    Lexer lex;
    static const std::string digits;
};


#endif //LAB3_3_BUILDER_H