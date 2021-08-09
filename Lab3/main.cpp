//variant 12
//Vladyslav Postrelko, K-13;
#include <iostream>
#include <cstring>
#include <stdexcept>
#include <vector>
#include "copyright.h"
#include "Builder.h"
#include "Info.h"

using namespace std;

using InfoResults=ostream& (ostream&, Info&);

//processes the command output
ostream& output(ostream& out, Info& inf){
    Output_Info::output(out, inf);
    return out;
}

//processes the command stat
ostream& stat(ostream& out, Info& inf) {
    Stat_Info::stat(out, inf);
    return  out;
}

//processes the command output or stat
bool do_command(Info& i, const char* fname, InfoResults f){
    if (strcmp(fname, "#con")==0) return bool(f(cout, i));
    ofstream ff(fname);
    bool res=bool(f(ff, i));
    if (ff.fail()) res=false;
    ff.close();
    return res;
}

//handles the input file
bool input(Info& inf, const char* filename){
    Builder build;
    bool b=true;
    cout<<"input "<<filename<<" : ";
    build.loadData(inf, filename);
    cout<<"OK"<<"\n";
    return b;
}

void stars() {cout<<"*****\n";}

void print0() {
    stars();
    cout<<"nothing to do\n";
    stars();
}

void help(){
    cout<<"<executable file> <input file> {-<comand> <output file>}\n";
}

void undef_output(){
    cout<<"output undefined\n";
}

void undef_stat(){
    cout<<"stat undefined\n";
}

void ignore_(const char *message){
    cout<<message<<" : ignored\n";
}

void cpair(int command, const char* filename, Info &inf){
    if (command==1) cout << "output " << filename << " : " << (do_command(inf, filename, output) ? "OK" : "UPS") << "\n";
    if (command==2) cout << "stat " << filename << " : " << (do_command(inf, filename, stat) ? "OK" : "UPS") << "\n";
}

//processes commands
void processing_request(int argc, char **argv, Info &inf) {
    unsigned long int i = 2;
    vector<int> arr(2);
    while (i < (unsigned long int)argc) {
        if (strcmp(argv[i], "-output") == 0) { arr.push_back(1); }
        else if (strcmp(argv[i], "-stat") == 0) { arr.push_back(2); }
        else (arr.push_back(0));
        i++;
    }
    i = 2;
    while (i < arr.size()) {
        if ((i != (arr.size() - 1)) && arr[i] && !arr[i + 1]) {//lazy evaluation
            cpair(arr[i], argv[i + 1], inf);
            i += 2;
        } else if (arr[i] == 1) {
            undef_output();
            i++;
        }
        else if (arr[i] == 2) {
            undef_stat();
            i++;
        }
        else {
            ignore_(argv[i]);
            i++;
        }
    }
}

int main(int argc, char **argv) {
    copyright();
    if (argc==1) {print0(); return 0;}
    if (strcmp(argv[1],"help")==0) {help(); return 0;}
    Info inf;
    try {
        stars();
        if (!input(inf, argv[1])) {
            stars();
            return 0;
        }
        processing_request(argc, argv, inf);
    }
    catch (BuilderError &ex) {inf.clear(); cout<<"UPS\n";cout<<ex.what();cout<<'\n';}
    catch (exception &ex) {inf.clear(); cout<<"UPS\n";cout<<ex.what();cout<<'\n';}
    catch (...) {inf.clear(); cout<<"UPS\n";cout<<"smth wrong\n";}
    stars();
    return 0;
}