/*
 * Matrix.h
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

#ifndef MYML_MATRIX_H
#define MYML_MATRIX_H

#include <cppMLKit/myVector.h>
#include <math.h>

template <typename T>
class Matrix{
    MyVector<MyVector<T>> data;
    size_t rows;
    size_t cols;
public:
    Matrix() : rows(0), cols(0) {}

    template <typename U>
    Matrix(const Matrix<U> &other) : data(other.getRows(), MyVector<T>(other.getCols(), T())),
        rows(other.getRows()), cols(other.getCols()) {
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                data[i][j] = T(other[i][j]);
            }
        }
    }
    Matrix(size_t rows, size_t cols, T val = 0) : data(rows, MyVector<T>(cols, val)),
        rows(rows), cols(cols) {}

    // void initMatrix(unsigned int rows, unsigned int cols);

    size_t getRows() const {return rows;}
    size_t getCols() const {return cols;}

    template <typename U>
    U getOneCol(unsigned int col) const {
        assert(col< cols);
        U ret(rows,0);
        for(int i=0; i<rows; ++i){
            ret[i] = data[i][col];
        }
        return ret;
    }

    template <typename U>
    void push_back(MyVector<U>& val) {
        data.push_back(val);
        rows++;
    }

    Matrix transpose() const {

        Matrix mT (cols, rows);

        for(int i=0; i<rows; ++i){
            for(int j=0; j<cols; ++j){
                mT[j][i] = data[i][j];
            }
        }
        return mT;
    }


    Matrix sigmoid() const {
        Matrix sigMat (rows, cols);
        for(int i=0; i<rows; ++i){
            for(int j=0; j<cols; ++j){
                sigMat[i][j] = 1.0/(1+exp(data[i][j]));
            }
        }
        return sigMat;
    }

    MyVector<T>& operator [] (int idx) {
        return data[idx];
    }
    const MyVector<T>& operator [] (int idx) const {
        return data[idx];
    }

    Matrix operator + (const Matrix &other) const{
        assert( rows == other.rows && cols == other.cols);
        Matrix ret(rows, cols);
        for(int i=0; i<rows; ++i){
            for(int j=0; j<cols; ++j){
                ret[i][j] = data[i][j] + other[i][j];
            }
        }
        return ret;
    }

    Matrix operator - (const Matrix &other) const{
        assert( rows == other.rows && cols == other.cols);
        Matrix ret(rows, cols);
        for(int i=0; i<rows; ++i){
            for(int j=0; j<cols; ++j){
                ret[i][j] = data[i][j] - other[i][j];
            }
        }
        return ret;
    }

    template <typename U>
    Matrix operator *= (const U val) {
        for(int i=0; i< rows; ++i){
            for(int j=0; j< cols; ++j){
                data[i][j] *= val;
            }
        }

        return *this;
    }

    template<typename V, typename U>
    friend Matrix<V> operator * (Matrix<V> lhs, const U val);
    template<typename V, typename U>
    friend Matrix<V> operator * (const U val, const Matrix<V>& rhs);

    Matrix operator * (const Matrix &other) const {

        assert(cols == other.rows);

        Matrix ret(rows, other.cols, 0);

        for(int i = 0; i<rows; ++i){
            for(int j=0; j<other.cols; ++j){
                for(int k=0; k<other.cols; ++k){
                    ret[i][j] += data[i][k] * other[k][j];
                }
            }
        }

        return ret;
    }

    template <typename U>
    friend std::istream& operator >> (std::istream& in, Matrix<U>& vec);

    template <typename U>
    friend std::ostream& operator << (std::ostream& out, const Matrix<U>& vec);
};

template<typename T, typename U>
Matrix<T> operator * (Matrix<T> lhs, const U val) {
    lhs *= val;
    return lhs;
}

template<typename T, typename U>
Matrix<T> operator * (const U val, const Matrix<T>& rhs) {
    return rhs * val;
}

template <typename T>
std::istream& operator >> (std::istream& in, Matrix<T>& mat)
{
    //char comma = 0;
    MyVector<T> d;
    bool isFirst = true;
    size_t s = 0;

    while (in>>d) {

        if (isFirst) {
            s = d.size();
            isFirst = false;
        }
        //std::cout<<d.size()<<std::endl;
        if (s != d.size()) {
            std::cerr<<"Each row should have same length"<<std::endl;
            exit(0);
        }

        mat.data.push_back(d);
    }

    mat.rows = mat.data.size();
    mat.cols = s;

    return in;
}

template <typename T>
std::ostream& operator << (std::ostream& out, const Matrix<T>& mat)
{
    bool isFirst = true;
    for (auto i = mat.data.begin(); i != mat.data.end(); ++i) {
        if (!isFirst) {
            out<<std::endl;
        } else {
            isFirst = false;
        }

        out<<*i;
    }

    return out;
}



#endif //MYML_MATRIX_H
