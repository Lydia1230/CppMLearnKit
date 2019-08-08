/*
 * LogisticRegression.h
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


#ifndef MYML_LOGISTICREGRESSION_H
#define MYML_LOGISTICREGRESSION_H
#include <cppMLKit/Matrix.h>

template <typename T>
T sigmoid(T val){
    return 1.0/(1+exp(-val));
}

//梯度上升
template <typename T, typename U>
Matrix<T> gradAscent(Matrix<T> dataMat, Matrix<U> classLabels){
    if(classLabels.getCols() != 1){
        std::cerr<<"label should be 1 column"<<std::endl;
        exit(0);
    }
    int m = dataMat.getRows();
    int n = dataMat.getCols();
    //std:: cout<< m<<" "<<n<<std::endl;

    Matrix<T> dataT = dataMat.transpose();
    Matrix<T> weights (n,1,1);
    float alpha  = 0.005;
    int maxCycles = 500;

    Matrix<T> z(m,1);
    //Matrix
    for(int k=0; k<maxCycles; ++k){
        z = dataMat * weights;
        z = z.sigmoid();

        z = classLabels - z;

        weights = weights + alpha*(dataMat.transpose()* z);
    }
    return weights;
}

//随机梯度上升
template <typename T, typename U>
MyVector<T> stocGradAscent(Matrix<T> dataMat, Matrix<U> classLabels){
    if(classLabels.getCols() != 1){
        std::cerr<<"label should be 1 column"<<std::endl;
        exit(0);
    }
    int m = dataMat.getRows();
    int n = dataMat.getCols();
    MyVector<T> weights(n,1);
    T h, error;
    float alpha  = 0.01;
    for(int i=0; i<m; ++i){

        h = sigmoid((dataMat[i] * weights).sum());

        error = classLabels[i][0] - h;
        weights = weights + alpha * error * dataMat[i];
    }
    return weights;
}


#endif //MYML_LOGISTICREGRESSION_H
