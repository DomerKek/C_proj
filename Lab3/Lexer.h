//Vladyslav Postrelko, K-13;
#ifndef LAB3_1_LEXER_H
#define LAB3_1_LEXER_H

#include <string>

enum class LineType {Empty, Footer, Header, Line};

class Lexer {
private:
    size_t pos, start, stop;
    std::string line;
    bool is_eof, is_first_field, is_empty;
    static const std::string separators, space;
    bool highlighter(size_t &begin, size_t &end);  //determines the position of the next field
public:
    Lexer ();  //if the user forgets to call load before using next. eof() return true
    LineType load(const std::string &str);  //gets the string you want to break into fields
    bool next(std::string &field);  //selects the next field from the line, return false if the field has ended
    bool eof() const noexcept;   //return true if the field has ended
};
//END_OF_LEXER;

#endif //LAB3_1_LEXER_H