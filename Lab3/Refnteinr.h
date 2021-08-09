//Vladyslav Postrelko, K-13;
#ifndef LAB3_REFNTEINR_H
#define LAB3_REFNTEINR_H

#include <stdexcept>
#include <iostream>

template <class T>
class Refnteinr {
public:
    using sizetype=size_t;
private:
    class base_Iterator{
    protected:
        T** cur;
    public:
        base_Iterator(){cur=nullptr;}
        base_Iterator(T** p) {cur=p;}
        bool operator==(const base_Iterator& it) const { return cur==it.cur;}
        bool operator!=(const base_Iterator& it) const { return cur!=it.cur;}
    };
public:
    class Iterator:public base_Iterator{
    public:
        Iterator()= default;
        explicit Iterator(T** p){base_Iterator::cur=p;}
        Iterator(const Iterator& it){base_Iterator::cur=it.cur;}
        Iterator&operator=(const Iterator& it) {base_Iterator::cur=it.cur; return *this;}

        Iterator& operator++(){++base_Iterator::cur; return *this;}
        Iterator& operator--(){--base_Iterator::cur; return *this;}
        Iterator operator++(int value){T** tmp=base_Iterator::cur; base_Iterator::cur++; return Iterator(tmp);}
        Iterator operator--(int value){T** tmp=base_Iterator::cur; Iterator::cur--; return Iterator(tmp);}

        Iterator& operator+(sizetype value){base_Iterator::cur+=value; return *this;}
        Iterator& operator-(sizetype value){base_Iterator::cur-=value; return *this;}

        T&operator*(){return **base_Iterator::cur;}
    };
    class const_Iterator: public base_Iterator{
    public:
        const_Iterator()=default;
        explicit const_Iterator(T** p){base_Iterator::cur=p;}
        const_Iterator(const const_Iterator& it){base_Iterator::cur=it.cur;}
        const_Iterator&operator=(const const_Iterator& it) {base_Iterator::cur=it.cur;}

        const_Iterator& operator++(){++base_Iterator::cur; return *this;}
        const_Iterator& operator--(){--base_Iterator::cur; return *this;}
        const_Iterator operator++(int value){T** tmp=base_Iterator::cur; base_Iterator::cur++; return const_Iterator(tmp);}
        const_Iterator operator--(int value){T** tmp=base_Iterator::cur; base_Iterator::cur--; return const_Iterator(tmp);}

        const_Iterator& operator+(sizetype value){base_Iterator::cur+=value; return *this;}
        const_Iterator& operator-(sizetype value){base_Iterator::cur-=value; return *this;}

        const T&operator*() const{ return **base_Iterator::cur;}
    };
    class reverse_Iterator: public base_Iterator{
    public:
        reverse_Iterator()= default;
        explicit reverse_Iterator(T** p){base_Iterator::cur=p;}
        reverse_Iterator(const reverse_Iterator& it){base_Iterator::cur=it.cur;}
        reverse_Iterator&operator=(const reverse_Iterator& it) {base_Iterator::cur=it.cur;}

        reverse_Iterator& operator++(){--base_Iterator::cur; return *this;}
        reverse_Iterator& operator--(){++base_Iterator::cur; return *this;}
        reverse_Iterator operator++(int value){T** tmp=base_Iterator::cur; base_Iterator::cur--; return reverse_Iterator(tmp);}
        reverse_Iterator operator--(int value){T** tmp=base_Iterator::cur; base_Iterator::cur++; return reverse_Iterator(tmp);}

        reverse_Iterator& operator+(sizetype value){base_Iterator::cur-=value; return *this;}
        reverse_Iterator& operator-(sizetype value){base_Iterator::cur+=value; return *this;}

        T&operator*(){return **base_Iterator::cur;}
    };
    class const_reverse_Iterator: public base_Iterator{
    public:
        const_reverse_Iterator()=default;
        explicit const_reverse_Iterator(T** p){base_Iterator::cur=p;}
        const_reverse_Iterator(const const_reverse_Iterator& it){base_Iterator::cur=it.cur;}
        const_reverse_Iterator&operator=(const const_reverse_Iterator& it) {base_Iterator::cur=it.cur;}

        const_reverse_Iterator& operator++(){--base_Iterator::cur; return *this;}
        const_reverse_Iterator& operator--(){++base_Iterator::cur; return *this;}
        const_reverse_Iterator operator++(int value){T** tmp=base_Iterator::cur; base_Iterator::cur--; return const_reverse_Iterator(tmp);}
        const_reverse_Iterator operator--(int value){T** tmp=base_Iterator::cur; base_Iterator::cur++; return const_reverse_Iterator(tmp);}

        const_reverse_Iterator& operator+(sizetype value){base_Iterator::cur-=value; return *this;}
        const_reverse_Iterator& operator-(sizetype value){base_Iterator::cur+=value; return *this;}

        const T&operator*() const{ return **base_Iterator::cur;}
    };
private:
    T** main_pointer;
    sizetype refnteinr_size;
    sizetype cpct;
    void qsort(T** m, sizetype l, sizetype r);
public:
    //MEMBER_FUNCTIONS
    Refnteinr();
    Refnteinr(sizetype value);
    Refnteinr(const Refnteinr& value);
    Refnteinr(Refnteinr&& value);
    Refnteinr& operator=(const Refnteinr &value);
    ~Refnteinr();
    //END_OF_MEMBER_FUNCTIONS

    //ELEMENT_ACCESS
    T& operator[](sizetype value);
    const T& operator[](sizetype value) const;
    T& at(sizetype value);
    const T& at(sizetype value) const;
    //END_OF_ELEMENT_ACCESS

    //ITERATOR
    Iterator begin(){return Iterator(main_pointer);}
    Iterator end(){return Iterator(main_pointer+refnteinr_size);}
    const_Iterator cbegin() const { return const_Iterator(main_pointer);}
    const_Iterator cend() const {return const_Iterator(main_pointer+refnteinr_size);}
    reverse_Iterator rbegin(){return reverse_Iterator(main_pointer+refnteinr_size-1);}
    reverse_Iterator rend(){return reverse_Iterator(main_pointer-1);}
    const_reverse_Iterator crbegin() const {return const_reverse_Iterator(main_pointer+refnteinr_size-1);}
    const_reverse_Iterator crend() const {return const_reverse_Iterator(main_pointer-1);}
    //END_OF_ITERATOR

    //CAPACITY
    sizetype size() const { return refnteinr_size;}
    sizetype capacity() const {return cpct;}
    bool empty() const { return !bool(refnteinr_size);}
    //END_OF_CAPACITY

    //MODIFIERS
    void push_back(const T &obj);
    void push_back(const T &&obj);
    void pop_back();
    void clear();
    void sort();
    //END_OF_MODIFIERS

    //LOOKUP
    auto find(const T& element, sizetype a=0) const;
    //END_OF_LOOKUP
};

#include "Refnteinr_impl.h"



#endif //LAB3_REFNTEINR_H


/*   main_pointer ----->  pointer[1]   pointer[2]  ...  pointer [i]
*                            |            |                |
*                            |            |                |
*                            |            |                |
*                            V            V                V
*                          DATA_1        DATA_2           DATA_i
*/