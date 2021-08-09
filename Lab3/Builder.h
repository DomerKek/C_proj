//Vladyslav Postrelko, K-13;
#ifndef LAB3_BUILDER_H
#define LAB3_BUILDER_H
#include "Info.h"
#include "Lexer.h"
#include <fstream>
#include <string>
#include <cstring>
#include <map>

typedef unsigned long int ulint;

class BuilderError:public std::exception{
private:
    //std::string message;
    const char* smal_message, *filename;
    static const std::map<int, const char*> explanation;
    const char* error_explanation(int error_number);
    void set_smal_message(const char* msg);
    int error_number, number_of_line;
public:
    const char* what() const noexcept;
    BuilderError(int error_number, int number_of_line, const char* msg, const char* flnm);
};


class Builder {
public:
    void loadData(Info& inf, const char* filename);
private:
    void processing(); //processing file line
    void do_header(); //processing header
    void do_footer(); //processing footer
    void do_line();//processing line
    void next_line(std::string &s);
    void error(int error_number=0,const char* msg="");
    int define_error_number();
    void chek_number_of_line();
    void chek();
    std::ifstream* pfilestream;
    Info* info;
    ulint strtoulint(std::string &str); //convert string to unsigned long int
    ulint max_prcnt, min_prcnt, amnt_footer, amnt_header, amnt_lines, amnt_of_tasks;
    ulint error_stoi_pos;
    std::string tmp, field;
    std::string fields[9];
    LineType type_of_line;
    int number_of_line;
    Lexer lex;
    const char* filename;
};


#endif //LAB3_BUILDER_H
