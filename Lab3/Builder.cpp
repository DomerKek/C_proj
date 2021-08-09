//Vladyslav Postrelko, K-13;
#include "Builder.h"

using namespace std;

const map<int, const char*> BuilderError::explanation={{100, " no header"},
                                                       {101, " there is more than one header"},
                                                       {102, " header does not match the contents of the file"},
                                                       {103, " inconsistent number of fields in header"},
                                                       {104, " another header error"},
                                                       {200, " no footer"},
                                                       {201, " there is more than one footer"},
                                                       {202, " footer does not match the contents of the file"},
                                                       {203, " inconsistent number of fields in footer"},
                                                       {204, " another footer error"},
                                                       {300, " the order of header-lines-footer is broken"},
                                                       {301, " incorrect numbering of lines"},
                                                       {302, " inappropriate number of fields in line"},
                                                       {303, " other errors in the line"},
                                                       {400, " error working with a physical file"},
                                                       {500, " bad allocation"}};


BuilderError::BuilderError(int error_number, int number_of_line, const char *msg, const char *flnm) {
    this->error_number=error_number;
    this->filename=flnm;
    set_smal_message(msg);
    if ((error_number>=300) && (303>=error_number)) this->number_of_line=number_of_line;
    else this->number_of_line=-1;
}

const char *BuilderError::error_explanation(int error_number) {
    auto it=explanation.find(error_number);
    if (it!=explanation.end()){
        return (*it).second;
    }
    const char *c="unknown Builder error";
    return c;
}

const char *BuilderError::what() const noexcept {
    string tmp=to_string(error_number)+" "+to_string(number_of_line)+" "+string(smal_message)+" in file: "+string(filename);
    char smth[tmp.size()];
    strcpy(smth, tmp.c_str());
    const char* message=smth;
    return message;
}

//const char *BuilderError::what() const noexcept {
//    string tmp=to_string(error_number)+" "+to_string(number_of_line)+" "+string(smal_message)+" in file: "+string(filename);
//    return tmp.c_str();
//}

void BuilderError::set_smal_message(const char* msg) {
    if (msg[0]=='\0') smal_message=error_explanation(error_number);
    else smal_message=msg;
}

void Builder::loadData(Info &inf, const char *filename) {
    this->filename=filename;
    this->info=&inf;
    info->clear();
    pfilestream=new ifstream;
    pfilestream->open(filename);
    if (!pfilestream->is_open()) {error(400);}
    amnt_footer=0; amnt_header=0; number_of_line=0; amnt_lines=0;
    try {
        while (!pfilestream->fail()) {
            processing();
        }
        chek();
    }
    catch (bad_alloc& ex) {error(500);}
    pfilestream->close();
    delete pfilestream;
}

void Builder::next_line(std::string &s) {
    number_of_line++;
    s.clear();
    getline(*pfilestream, s);
}

void Builder::processing() {
    next_line(tmp);
    type_of_line = lex.load(tmp);
    if (type_of_line == LineType::Header) { do_header(); }
    else if (type_of_line == LineType::Line) { do_line(); }
    else if (type_of_line == LineType::Footer) { do_footer(); }
}

void Builder::do_header() {
    if (!amnt_header) {
        amnt_header++;
        int cnt=0;
        try{
            if (lex.next(field)) {
                min_prcnt=strtoulint(field);
                cnt++;
            }
            if (lex.next(field)) {
                max_prcnt=strtoulint(field);
                cnt++;
            }
        }
        catch (invalid_argument &ex){error(104, ex.what());}
        if (cnt!=2 || !lex.eof()){error(103);}
    }
    else error(101);
}

void Builder::do_footer() {
    if (!amnt_lines || !amnt_header) error(300);
    if (!amnt_footer) {
        amnt_footer++;
        int cnt=0;
        if (lex.next(field)) {
            amnt_of_tasks=strtoulint(field);
            cnt++;
        }
        if (cnt!=1 || !lex.eof()){error(203);}
    }
    else {error(201);}
}

void Builder::do_line() {
    if (!amnt_header) error(100);
    if (amnt_footer) error (300);
    amnt_lines++;
    int i=0;
    while (i<9 && lex.next(fields[i]))i++;
    if (i!=9 || !lex.eof()) {error(302);}
    chek_number_of_line();
    try{
        info->load(strtoulint(fields[1]), strtoulint(fields[2]), fields[3], fields[4], fields[5], fields[6], strtoulint(fields[7]), strtoulint(fields[8]));
    }
    catch (invalid_argument &ex){error(303, ex.what());}
}

ulint Builder::strtoulint(std::string &str) {
    ulint result=0;
    size_t pos;
    try {
        result=(stoul(str, &pos));
    }
    catch (exception& ex){error();}
    if (pos!=str.size()) error();
    return result;
}

void Builder::error(int error_number, const char* msg) {
    if (!error_number) error_number=define_error_number();
    throw BuilderError(error_number, number_of_line, msg, filename);
}

int Builder::define_error_number() {
    if (type_of_line==LineType::Header) return 104;
    else if (type_of_line==LineType::Footer) return 204;
    return 303;
}

void Builder::chek_number_of_line() {
    if (amnt_lines!=strtoulint(fields[0])){error(301);}
}

void Builder::chek() {
    if (!amnt_footer) error(200);
    if (max_prcnt!=info->get_max_prct() || min_prcnt!=info->get_min_prct()) error(102);
    if (amnt_of_tasks!=(ulint)info->get_size()) error(202);
}


