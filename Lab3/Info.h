//Vladyslav Postrelko, K-13;
#ifndef LAB3_3_INFO_H
#define LAB3_3_INFO_H

#include <string>
#include <vector>
#include <exception>
#include <iostream>
#include "Refnteinr.h"

typedef unsigned long int ulint;

class Info {//all data
private:
    class Task{//class that stores the attempts of this task
    private:
        class Attempt{//student attempt class, details
        private:
            class Date{//class to store the date. controls all errors that may result from the wrong date
            private:
                ulint day, month, year;
            public:
                ulint get_day() const noexcept { return day;}
                ulint get_month() const noexcept { return month;}
                ulint get_year() const noexcept { return year;}
                void set_year(ulint y);
                void set_month(ulint m);
                void set_day(ulint d);
                Date(ulint d, ulint m, ulint y);
                Date():day(1), month(1), year(2001){}
            };
            std::string name;
            std::string surname;
            std::string patronymic;
            static const std::string name_cheker;
            ulint percent;
            Date date;
            void set_full_name(std::string &srnm, std::string &nm, std::string &ptrnmc);
            void set_percent(ulint prct);
        public:
            ulint get_percent() const noexcept { return percent;}
            std::string get_name() const noexcept {return name;}
            std::string get_surname() const noexcept {return surname;}
            std::string get_patronymic() const noexcept {return patronymic;}
            Date get_date() const noexcept {return date;}
            Attempt()= default;
            Attempt(ulint m, ulint d, std::string &srnm, std::string &nm, std::string &ptrnmc, ulint prct, ulint y);
            bool is_less_then_50() const { return percent<=50;}
            bool is_big_then_30() const { return percent>30;}
            bool operator>(const Attempt &a)const;
            bool operator==(const Attempt &a)const;
            bool operator<(const Attempt &a)const { return !(((*this)>a) || ((*this)==a));}
            bool operator!=(const Attempt &a)const{return !((*this)==a);}
            bool operator>=(const Attempt &a)const { return (*this)>a || (*this)==a;}
            bool operator<=(const Attempt &a)const { return (*this)<a || (*this)==a;}
        };
        std::string task;
        static const std::string task_cheker;
        Refnteinr <Attempt> attempts;
        ulint amount_of_attempts_more_30_prct, prct_of_the_best_attempt, prct_of_the_worst_attempt;
        ulint amnt_of_attmpts_more_30_prct();
        bool is_more_50_prct();
        ulint the_best_attempt();
        ulint the_worst_attempt();
        void update(); //update value of fields: amount_of_attempts_more_30_prct, prct_of_the_best_attempt, prct_of_the_worst_attempt, is_results_more_then_50_prct;
        bool is_results_more_then_50_prct;
    public:
        void clear(){attempts.clear();}
        bool get_is_results_more_then_50_prct() const noexcept { return is_results_more_then_50_prct;}
        ulint get_prct_of_the_best_attempt() const noexcept { return prct_of_the_best_attempt;}
        ulint get_prct_of_the_worst_attempt() const noexcept {return prct_of_the_worst_attempt;}
        ulint get_amount_of_attempts_more_30_prct() const noexcept { return amount_of_attempts_more_30_prct;}
        ulint get_ammount_of_attempts() const noexcept { return ulint (attempts.size());}
        std::string get_task() const noexcept { return task;}
        void load(ulint m, ulint d, std::string &srnm, std::string &nm, std::string &ptrnmc, ulint prct, ulint y);//creates an object of class attempt
        size_t size() const {return attempts.size();}
        void sort_attempts(){attempts.sort();}
        Task()= default;
        Task(std::string &tsk);
        bool operator>(const Task &a) const;
        bool operator==(const Task &a) const;
        bool operator<(const Task &a)const{return !(((*this)>a) || ((*this)==a));}
        bool operator!=(const Task &a)const {return !((*this)==a);}
        bool operator>=(const Task &a)const { return (*this)>a || (*this)==a;}
        bool operator<=(const Task &a) const{ return (*this)<a || (*this)==a;}

        friend class Output_Info;
        friend class Stat_Info;
    };
    Refnteinr <Task> tasks;
    size_t find(std::string &tsk) const;
public:
    void clear();
    void sort_tasks(){tasks.sort();}
    ulint get_max_prct() const noexcept;
    ulint get_min_prct() const noexcept;
    bool load (std::string &tsk);//creates a new object Task
    void load (ulint mm, ulint dd, std::string &srnm, std::string &tsk, std::string &nm, std::string &ptrnmc, ulint prct, ulint yy);//adds a try to Task if such Task already exists. otherwise it creates a new Task and adds an attempt to it.
    Info()= default;
    size_t get_size() const { return tasks.size();}
    bool is_empty() const {return tasks.empty();}
    friend class Output_Info;
    friend class Stat_Info;
};

class Output_Info{
public:
    static void output(std::ostream& os, Info& inf);
private:
    static void print_specifics (std::ostream& os, Info::Task& tsk, ulint &cnt);
};

class Stat_Info{
public:
    static void stat(std::ostream& os, Info& inf);
private:
    static void print_specifics(std::ostream& os, Info::Task& tsk);
};


#endif //LAB3_3_INFO_H