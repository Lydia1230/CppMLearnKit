/*
 * myVector.h
 *
 * Copyright (C) 2019 Jingying Li<jyli1230@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef MYML_MYVECTOR_H
#define MYML_MYVECTOR_H

#include <iostream>
#include <vector>
#include <cstdlib>
#include <sstream>

template <typename T>
class MyVector : public std::vector<T>
{
public:
    using std::vector<T>::vector;

    MyVector operator * (const MyVector &other) const{
        assert(this->size() == other.size());

        MyVector ret(this->size());
        for(int i=0; i<this->size(); ++i){
            ret[i] = this->at(i) * other[i];
        }

        return ret;
    }

    T sum() {
        T ret(0);
        for (auto i = this->begin(); i != this->end(); ++i) {
            ret += *i;
        }
        return ret;
    }

    MyVector operator + (const MyVector &other) const{
        assert( this->size() == other.size());
        MyVector ret(this->size(),0);
        for(int i=0; i<this->size(); ++i){

            ret[i] = this->at(i) + other[i];

        }
        return ret;
    }

    MyVector operator - (const MyVector &other) const{
        assert( this->size() == other.size());
        MyVector ret(this->size(),0);
        for(int i=0; i<this->size(); ++i){

            ret[i] = this->at(i) - other[i];

        }
        return ret;
    }

    template <typename U>
    MyVector operator *= (const U val) {
        for(int i=0; i< this->size(); ++i){
            this->at(i) *= val;
        }
        return *this;
    }

    template<typename V, typename U>
    friend MyVector<V> operator * (MyVector<V> lhs, const U val);
    template<typename V, typename U>
    friend MyVector<V> operator * (const U val, const MyVector<V>& rhs);


    template <typename U>
    friend std::istream& operator >> (std::istream& in, MyVector<U>& vec);

    template <typename U>
    friend std::ostream& operator << (std::ostream& out, const MyVector<U>& vec);
};

template<typename T, typename U>
MyVector<T> operator * (MyVector<T> lhs, const U val) {
    lhs *= val;
    return lhs;
}

template<typename T, typename U>
MyVector<T> operator * (const U val, const MyVector<T>& rhs) {
    return rhs * val;
}

template <typename T>
std::istream& operator >> (std::istream& in, MyVector<T>& vec)
{
    vec.clear();
    char comma = 0;
    T val;
    while (in>>val) {
        //std:: cout<<val<<std:: endl;
        vec.push_back(val);

        if (!(in.get(comma))) {
            break;
        }
        //std:: cout<<comma<<std:: endl;
        if (comma == 0 || comma == 13 || comma =='\n') {
            break;
        }
        if (comma != ',') {
            std::cerr << "Data must be splited by comma!" << std::endl;
            exit(0);
        }
    }

    return in;
}

template <>
inline std::istream& operator >> <std::string>(std::istream& in, MyVector<std::string>& vec)
{
    vec.clear();
    char comma = 0;
    std::stringstream ss;
    std::string line;
    in>>line;
    ss.str(line);
    char delim = ',';
    std::string item;
    while(getline(ss,item,delim)){
        vec.push_back(item);
    }

    /*while (in>>val) {
        //std:: cout<<val<<std:: endl;
        vec.push_back(val);

        if (!(in.get(comma))) {
            break;
        }
        //std:: cout<<comma<<std:: endl;
        if (comma == 0 || comma == 13 || comma =='\n') {
            break;
        }
        if (comma != ',') {
            std::cerr << "Data must be splited by comma!" << std::endl;
            exit(0);
        }
    }*/

    return in;
}

template <typename T>
std::ostream& operator << (std::ostream& out, const MyVector<T>& vec)
{
    bool isFirst = true;
    for (auto i = vec.begin(); i != vec.end(); ++i) {
        if (!isFirst) {
            out<<',';
        } else {
            isFirst = false;
        }

        out<<*i;
    }

    return out;
}

#endif //MYML_MYVECTOR_H
