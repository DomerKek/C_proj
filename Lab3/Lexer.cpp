//Vladyslav Postrelko, K-13;
#include "Lexer.h"

using namespace std;

const string Lexer::separators="?!>=";
const string Lexer::space="\011\012\013\014\015\040";

Lexer::Lexer() : is_eof(true){}

bool Lexer::highlighter(size_t &begin, size_t &end) {
    if (pos==line.length()) {is_eof=true; return false;}
    pos++;
    begin=pos;
    end=pos;
    begin=line.find_first_not_of(space, pos);
    if (begin==string::npos) {begin=pos;}
    pos=line.find_first_of(separators, begin);
    if (pos==begin) {end=begin; return true;}
    if (pos==string::npos) {pos=line.length();}
    pos--;
    end=line.find_last_not_of(space, pos)+1;
    pos++;
    return true;
}

LineType Lexer::load(const std::string &tmp) {
    pos = size_t(-1);
    line = tmp;
    is_empty= false;
    is_eof= false;
    if ((line.find_first_not_of(space)==string::npos) || (line.empty())) {is_empty=true; return LineType::Empty;}
    start=0;
    stop=0;
    is_first_field=true;
    highlighter(start, stop);
    if (line.compare(start, stop-start, "footer")==0){if (pos==line.length()) {is_empty=true;} is_first_field=false; return LineType::Footer;}
    if (line.compare(start, stop-start, "header")==0){if (pos==line.length()) {is_empty=true;} is_first_field=false; return LineType::Header;}
    return LineType::Line;
}

bool Lexer::next(string &field){
    if (eof()) {return false;}
    if (is_empty) {is_eof=true; field=""; return true;}
    if (is_first_field) {is_first_field=false;}
    else {
        if (!highlighter(start, stop)) {is_eof=true; return false;};
    }
    field=line.substr(start, stop-start);
    if (pos == line.length()) is_eof=true;
    return true;
}

bool Lexer::eof() const noexcept {
    return is_eof;
}
