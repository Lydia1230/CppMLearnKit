/*
 * testDecisionTree.cpp
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
#include <cppMLKit/DecicionTree.h>

#include <fstream>
#include <string>

using namespace std;

int main(){
    cout<<"loading testSetIn.txt ... ";
    string filePath = "../data/data_decisionTree.txt";
    DecisionTree tree;
    tree.loadData(filePath);
    cout<<"loading finished!"<<endl;
    Matrix<int> ret = tree.getTrainDataMat();
    int m = ret.getRows(), n = ret.getCols();

    tree.buildTree(tree.getTrainDataMat(), tree.getAttrVec());

    vector<vector<string>> input ={{"rainy","69","92","TRUE"}};
    vector<string> ans = tree.predict(input);
    for(int i=0; i<ans.size(); ++i)
        cout<<ans[i]<<endl;

    return 0;
}

