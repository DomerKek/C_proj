//Vladyslav Postrelko, K-13;
#include <stdexcept>
#include "Info.h"

using namespace std;

const string Info::Task::Attempt::name_cheker="qwertyuiopasdfghjklzxcvbnm-'QWERTYUIOPASDFGHJKLZXCVBNM";
const std::string Info::Task::task_cheker="qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM 1234567890_{}$";

void Info::Task::Attempt::Date::set_year(ulint y) {
    if (y<2001 || y>9999) throw invalid_argument("invalid year");
    year=y;
}

void Info::Task::Attempt::Date::set_month(ulint m) {
    if (m==0 || m>12) throw invalid_argument("invalid month");
    month=m;
}
//30 days: 4 6 9 11
void Info::Task::Attempt::Date::set_day(ulint d) {
    bool b;
    if (month==4 || month==6 || month==9 || month==11){
        b=bool(d>30);
    }
    else if (month==2){
        if (year%4==0){
            b=bool(d>29);
        }
        else {b=bool(d>28);}
    }
    else {b=bool(d>31);}
    b=b||(d==0);
    if (b) throw invalid_argument("wrong day");
    day=d;
}

Info::Task::Attempt::Date::Date(ulint d, ulint m, ulint y) {
    set_year(y);
    set_month(m);
    set_day(d);
}

void Info::Task::Attempt::set_percent(ulint prct) {
    if (prct>100) throw invalid_argument("wrong percent");
    percent=prct;
}

void Info::Task::Attempt::set_full_name(std::string &srnm, std::string &nm, std::string &ptrnmc) {
    if (nm.empty() || srnm.empty() || ptrnmc.empty()){throw invalid_argument("empty field in full name!");}
    if (srnm.size()>30 || srnm.find_first_not_of(name_cheker)!=string::npos) {throw  invalid_argument("wrong surname");}
    if (nm.size()>26 || nm.find_first_not_of(name_cheker)!=string::npos) {throw  invalid_argument("wrong name");}
    if (ptrnmc.size()>30 || ptrnmc.find_first_not_of(name_cheker)!=string::npos) {throw invalid_argument("wrong patronymic");}
    name=nm; surname=srnm; patronymic=ptrnmc;
}

Info::Task::Attempt::Attempt(ulint m, ulint d, std::string &srnm, std::string &nm, std::string &ptrnmc, ulint prct, ulint y) {
    set_full_name(srnm, nm, ptrnmc);
    set_percent(prct);
    date=Date(d,m,y);
}

bool Info::Task::Attempt::operator>(const Info::Task::Attempt &a) const{
    if (this->percent>a.percent) {return true;}
    else if (this->percent<a.percent) { return false;}
    else{
        if (this->date.get_year()>a.date.get_year()){ return true;}
        else if (this->date.get_year()<a.date.get_year()) { return false;}
        else {
            if (this->surname.compare(a.surname)>0) { return true;}
            else if (this->surname.compare(a.surname)<0) {return false;}
            else{
                if (this->name.compare(a.name)>0) { return true;}
                else if (this->name.compare(a.name)<0) {return false;}
                else{
                    return (this->patronymic.compare(a.patronymic)>0);
                }
            }

        }
    }
}

bool Info::Task::Attempt::operator==(const Info::Task::Attempt &a)const {
    return this->percent==a.percent && this->date.get_year()==a.date.get_year() && this->surname.compare(a.surname)==0 && this->name.compare(a.name)==0 && this->patronymic.compare(a.patronymic) == 0;
}

void Info::Task::load(ulint m, ulint d, std::string &srnm, std::string &nm, std::string &ptrnmc, ulint prct, ulint y) {
    attempts.push_back(Attempt(m,d,srnm,nm,ptrnmc,prct,y));
    update();
}

ulint Info::Task::amnt_of_attmpts_more_30_prct() {
    ulint i=0, result=0;
    while (i<attempts.size()){result+=ulint(attempts[i].is_big_then_30()); i++;}
    amount_of_attempts_more_30_prct=result;
    return result;
}

bool Info::Task::is_more_50_prct() {
    ulint i=0, result=0;
    while (i<attempts.size()){result+=ulint(!attempts[i].is_less_then_50()); i++;}
    is_results_more_then_50_prct=bool(result);
    return bool(result);
}

ulint Info::Task::the_best_attempt() {
    ulint max=0, i=0;
    while (i<attempts.size()) {if (attempts[i].get_percent()>max) {max=attempts[i].get_percent();} i++;}
    prct_of_the_best_attempt=max;
    return max;
}

ulint Info::Task::the_worst_attempt() {
    ulint min=100, i=0;
    while (i<attempts.size()) {if (attempts[i].get_percent()<min) {min=attempts[i].get_percent();} i++;}
    prct_of_the_worst_attempt=min;
    return min;
}

void Info::Task::update() {
    if (!is_results_more_then_50_prct) {is_more_50_prct();}
    amnt_of_attmpts_more_30_prct();
    the_best_attempt();
    the_worst_attempt();
}

Info::Task::Task(std::string &tsk) {
    if (tsk.size()>67 || tsk.find_first_not_of(task_cheker)!=string::npos) throw invalid_argument("wrong task!");
    task=tsk;
}

bool Info::Task::operator>(const Info::Task &a) const {
    if (this->prct_of_the_best_attempt>a.prct_of_the_best_attempt) { return true;}
    else if (this->prct_of_the_best_attempt<a.prct_of_the_best_attempt) { return false;}
    else {
        if (this->get_ammount_of_attempts() > a.get_ammount_of_attempts()) { return true; }
        else if (this->get_ammount_of_attempts() < a.get_ammount_of_attempts()) { return false; }
        else {
            return this->task.compare(a.task)>0;
        }
    }
}

bool Info::Task::operator==(const Info::Task &a) const {
    return this->prct_of_the_best_attempt==a.prct_of_the_best_attempt && this->get_ammount_of_attempts() == a.get_ammount_of_attempts() && this->task.compare(a.task)==0;
}

size_t Info::find(std::string &tsk) const {
    for (size_t i=0; i<tasks.size(); i++){
        if (tsk == tasks[i].get_task()) {return i;}
    }
    return size_t(-1);
}

bool Info::load(std::string &tsk) {
    size_t pos=find(tsk);
    if (pos==size_t(-1)){
        tasks.push_back(tsk);
        return true;
    }
    return false;
}

void Info::load(ulint mm, ulint dd, std::string &srnm, std::string &tsk, std::string &nm, std::string &ptrnmc, ulint prct,
                ulint yy) {
    size_t pos=find(tsk);
    if (pos==size_t(-1)){
        tasks.push_back(tsk);
        pos=tasks.size()-1;
    }
    tasks[pos].load(mm,dd,srnm,nm,ptrnmc,prct,yy);
}

ulint Info::get_max_prct() const noexcept {
    ulint max=0, tmp;
    for (auto it=tasks.crbegin();it!=tasks.crend();++it){
        tmp=(*it).get_prct_of_the_best_attempt();
        if (tmp>max){max=tmp;}
    }
    return max;
}

ulint Info::get_min_prct() const noexcept {
    ulint min=100, tmp;
    for (auto it=tasks.crbegin();it!=tasks.crend();++it){
        tmp=(*it).get_prct_of_the_worst_attempt();
        if (tmp<min){min=tmp;}
    }
    return min;
}

void Info::clear() {
    for (auto it=tasks.begin();it!=tasks.end();++it){
        (*it).clear();
    }
    tasks.clear();
}

void Output_Info::output(std::ostream& os, Info& inf) {
    os<<"header > "<<inf.get_min_prct()<<" > "<<inf.get_max_prct()<<'\n';
    ulint cnt=1;
    for (auto it=inf.tasks.begin(); it!=inf.tasks.end(); ++it){
        print_specifics(os, (*it), cnt);
    }
    os<<"footer > "<<inf.tasks.size();
}

void Output_Info::print_specifics(std::ostream &os, Info::Task& tsk, ulint &cnt){
    for (auto it=tsk.attempts.begin() ; it!=tsk.attempts.end(); ++it){
        os<<cnt<<" > "<<(*it).get_date().get_month()
          <<" > "<<(*it).get_date().get_day()
          <<" > "<<(*it).get_surname()
          <<" > "<<tsk.get_task()
          <<" > "<<(*it).get_name()
          <<" > "<<(*it).get_patronymic()
          <<" > "<<(*it).get_percent()
          <<" > "<<(*it).get_date().get_year()<<'\n';
        cnt++;
    }
}

void Stat_Info::stat(std::ostream &os, Info& inf) {
    if (inf.is_empty()) return;
    inf.tasks.sort();
    for (auto it=inf.tasks.begin(); it!=inf.tasks.end(); ++it){
        if(!(*it).get_is_results_more_then_50_prct()){
            os<<(*it).get_task()<<' '<<(*it).get_amount_of_attempts_more_30_prct()<<' '<<(*it).get_ammount_of_attempts()<<' '<<(*it).get_prct_of_the_best_attempt()<<'\n';
            print_specifics(os, (*it));
        }
    }
}

void Stat_Info::print_specifics(std::ostream &os, Info::Task &tsk) {
    if (tsk.attempts.empty()) { return;}
    tsk.attempts.sort();
    for (auto it=tsk.attempts.cbegin();it!=tsk.attempts.end();++it){
        os<<'\t'<<(*it).get_surname()<<' '<<(*it).get_name()<<' '<<(*it).get_patronymic()<<' '<<(*it).get_percent()<<' '<<(*it).get_date().get_year()<<'\n';
    }
}