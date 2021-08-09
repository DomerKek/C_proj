//Vladyslav Postrelko, K-13;

template <class T>
Refnteinr<T>::Refnteinr() {
    refnteinr_size = 0;
    cpct=0;
}

template <class T>
Refnteinr<T>::Refnteinr(sizetype value):refnteinr_size(value), cpct(value) {
    main_pointer=new T*[value];
    for (sizetype i=0; i<value;++i) {main_pointer[i]=new T;}
}

template <class T>
T& Refnteinr<T>::operator[](sizetype value) {
    //if (value>=refnteinr_size) {throw out_of_range("invalid index");}
    return *(main_pointer[value]);
}

template <class T>
const T& Refnteinr<T>::operator[](sizetype value) const {
    //if (value>=refnteinr_size) {throw out_of_range("invalid index");}
    return *(main_pointer[value]);
}

template <class T>
Refnteinr<T>::Refnteinr(const Refnteinr &value) {
    cpct=value.capacity();
    refnteinr_size=value.size();
    main_pointer=new T*[value.capacity()];
    for (sizetype i=0;i<value.size();++i){
        main_pointer[i]=new T;
        *main_pointer[i]=*(value.main_pointer[i]);
    }
}

template<class T>
Refnteinr<T>::Refnteinr(Refnteinr &&value) {
    cpct=move(value.capacity());
    refnteinr_size=move(value.size());
    main_pointer=new T*[cpct];
    for (sizetype i=0;i<refnteinr_size;++i){
        main_pointer[i]=new T;
        *main_pointer[i]=move(*(value.main_pointer[i]));
    }
}

template <class T>
Refnteinr<T> &Refnteinr<T>::operator=(const Refnteinr &value) {
    T** p=new T*[value.capacity()];
    for (sizetype i=0;i<value.size();++i){
        p[i]=new T;
        (*p[i])=value[i];
    }
    for (sizetype i=0;i<cpct;++i){
        delete main_pointer[i];
    }
    if (cpct!=0) delete[] main_pointer;
    main_pointer=p;
    cpct=value.capacity();
    refnteinr_size=value.size();
    return *this;
}

template <class T>
void Refnteinr<T>::push_back(const T &obj) {
    if (cpct==0){cpct=1;refnteinr_size=1;main_pointer=new T*;main_pointer[0]=new T;*main_pointer[0]=obj;return;}
    if (cpct==refnteinr_size) {
        cpct*=2;
        T **p = new T *[cpct];
        sizetype i=0;
        for (i = 0; i < refnteinr_size; ++i) {
            //p[i] = new T;
            p[i] = (*this).main_pointer[i];
        }
        p[refnteinr_size]=new T;
        *p[refnteinr_size] = obj;
        delete[] main_pointer;
        main_pointer = p;
    }
    else{
        main_pointer[refnteinr_size]=new T;
        *main_pointer[refnteinr_size]=obj;
    }
    ++refnteinr_size;
}

template <class T>
Refnteinr<T>::~Refnteinr() {
    if (refnteinr_size!=0) {
        for (sizetype i = 0; i < refnteinr_size; ++i) {
            delete main_pointer[i];
        }
        delete[] main_pointer;
    }
}

template<class T>
T &Refnteinr<T>::at(Refnteinr::sizetype value) {
    if (value>=refnteinr_size) {throw std::out_of_range("invalid index");}
    return *(main_pointer[value]);
}

template<class T>
const T &Refnteinr<T>::at(Refnteinr::sizetype value) const {
    if (value>=refnteinr_size) {throw std::out_of_range("invalid index");}
    return *(main_pointer[value]);
}

template<class T>
void Refnteinr<T>::clear() {
    for (sizetype i=0;i<refnteinr_size;++i){
        delete main_pointer[i];
    }
    refnteinr_size=0;
}

template<class T>
void Refnteinr<T>::pop_back() {
    if (refnteinr_size!=0) {
        delete main_pointer[refnteinr_size - 1];
        refnteinr_size--;
    }
}

template<class T>
void Refnteinr<T>::qsort(T **m, Refnteinr::sizetype l, Refnteinr::sizetype r) {
    sizetype i = l;
    sizetype j = r;
    T* x = m[(l+r) / 2];
    do{
        while (*m[i] < *x) i++;
        while (*m[j] > *x) j--;
        if (i <= j)
        {
            T* tmp = m[i];
            m[i] = m[j];
            m[j] = tmp;
            i++;
            j--;
            if (j==sizetype(-1)) return;
        }
    }
    while (i <= j);
    if (l < j) qsort(m, l, j);
    if (i < r) qsort(m, i, r);
}

template<class T>
void Refnteinr<T>::sort() {
    Refnteinr::qsort(main_pointer, 0, refnteinr_size-1);
}

template<class T>
auto Refnteinr<T>::find(const T &element, sizetype a) const {
    sizetype rslt=sizetype(-1);
    for (sizetype i=a;i<refnteinr_size;++i){
        if (element==*main_pointer[i]) {rslt=i; break;}
    }
    return rslt;
}

template<class T>
void Refnteinr<T>::push_back(const T &&obj) {
    if (cpct==0){cpct=1;refnteinr_size=1;main_pointer=new T*;main_pointer[0]=new T(obj);*main_pointer[0]=obj;return;}
    if (cpct==refnteinr_size) {
        cpct*=2;
        T **p = new T *[cpct];
        sizetype i=0;
        for (i = 0; i < refnteinr_size; ++i) {
            p[i] = (*this).main_pointer[i];
        }
        p[refnteinr_size]=new T;
        *p[refnteinr_size] = obj;
        delete[] main_pointer;
        main_pointer = p;
    }
    else{
        main_pointer[refnteinr_size]=new T;
        *main_pointer[refnteinr_size]=obj;
    }
    ++refnteinr_size;
}
