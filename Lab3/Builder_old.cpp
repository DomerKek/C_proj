//Vladyslav Postrelko, K-13;
#include "Builder_old.h"

using namespace std;

const std::string Builder::digits="1234567890";

void Builder::loadData(Info &inf, const char *filename) {
    this->info=&inf;
    info->clear();
    ifstream filestream;
    pfilestream=&filestream;
    filestream.open(filename);
    if (!filestream.is_open()) {throw BuilderError(400, -1);}
    is_header= false; is_footer= false; is_line= false; is_max_prcnt= false; is_min_prcnt=false;
    number=1;
    number_of_line=-1;
    try {
        while (!filestream.fail()) {
            processing();
        }
    }
    catch (bad_alloc& ex) {throw BuilderError(500, -1);}
    if (!(is_min_prcnt && is_max_prcnt)) {throw BuilderError(102, -1);}
    if (!is_footer) {throw BuilderError(200, -1);}
    filestream.close();
    pfilestream= nullptr;
}

void Builder::processing() {
    tmp="";
    number_of_line++;
    getline(*pfilestream, tmp);
    type_of_line=lex.load(tmp);
    if (type_of_line==LineType::Empty){
        return;
    }
    else if (type_of_line==LineType::Header) {
        do_header();
        return;
    }
    else if (!is_header) {throw BuilderError(100, -1);}
    else if (type_of_line==LineType::Line){
        do_line();
        return;
    }
    else if (!is_line) {throw BuilderError(300, number_of_line);}
    else if (type_of_line==LineType::Footer) {
        do_footer();
        return;
    }
}

void Builder::do_header() {
    if (!is_header) {
        is_header=true;
        int cnt=0;
        try{
            if (lex.next(field)) {
                min_prcnt=strtouint(field);
                cnt++;
            }
            if (lex.next(field)) {
                max_prcnt=strtouint(field);
                cnt++;
            }
        }
        catch (invalid_argument &ex){throw BuilderError(104, -1, ex.what());}
        if (cnt!=2 || !lex.eof()){throw BuilderError(103, -1);}
    }
    else {throw BuilderError(101, -1);}
}

void Builder::do_footer() {
    if (!is_footer) {
        is_footer=true;
        int cnt=0;
        if (lex.next(field)) {
            auto t=strtouint(field);
            if (t!=info->get_size()){throw BuilderError(202, -1);}
            cnt++;
        }
        if (cnt!=1 || !lex.eof()){throw BuilderError(203, -1);}
    }
    else {throw BuilderError(201, -1);}
}

void Builder::do_line() {
    if (is_footer) {throw BuilderError(300, number_of_line);}
    is_line=true;
    int i=0;
    while (i<9 && lex.next(fields[i])){
        i++;
    }
    if (i!=9 || !lex.eof()) {throw BuilderError(302, number_of_line);}
    if (number==strtouint(fields[0])){number++;}
    else {throw BuilderError(301, number_of_line);}
    try{
        ulint prcnt=strtouint(fields[7]);
        if (prcnt>max_prcnt || prcnt<min_prcnt) {throw BuilderError(102, -1);}
        if (prcnt==max_prcnt){is_max_prcnt=true;}
        if (prcnt==min_prcnt){is_min_prcnt=true;}
        info->load(strtouint(fields[1]), strtouint(fields[2]), fields[3], fields[4], fields[5], fields[6], prcnt, strtouint(fields[8]));
    }
    catch (invalid_argument &ex){throw BuilderError(303, number_of_line, ex.what());}
}

ulint Builder::strtouint(std::string &str) {
    if (str.find_first_not_of(digits)==string::npos) {
        return ulint(stoi(str));
    }
    if (type_of_line==LineType::Header) throw BuilderError(104, -1);
    if (type_of_line==LineType::Footer) throw BuilderError(204, -1);
    if (type_of_line==LineType::Line) throw BuilderError(303, number_of_line);
    return ulint (-1);
}

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

BuilderError::BuilderError(int error_number, int number_of_line):BuilderError(error_number, number_of_line, error_explanation(error_number)){}

BuilderError::BuilderError(int error_number, int number_of_line, const char *msg) {
    this->error_number=error_number;
    this->number_of_line=number_of_line;
    smal_message=msg;
}

const char *BuilderError::error_explanation(int error_number) {
    auto it=explanation.find(error_number);
    if (it!=explanation.end()){
        return (*it).second;
    }
    const char *c="unknown Builder error";
    return c;
}

const char *BuilderError::what() {
    message=to_string(error_number)+" "+to_string(number_of_line)+" "+string(smal_message);
    return (message).c_str();
}
