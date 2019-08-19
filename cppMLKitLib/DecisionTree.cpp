/*
 * DecisionTree.cpp
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
using namespace std;

TreeNode* TreeNode:: getChild(std::string attrN) {
    auto it = children.find(attrN);
    if (it != children.end()) {
        return it->second;
    }
    return nullptr;
}

std::vector<TreeNode*> TreeNode::getChildrenPointers(){
   std:: vector<TreeNode*> ret;
   for(auto it = children.begin(); it!= children.end(); ++it){
       ret.push_back(it->second);
   }
   return ret;
}



void DecisionTree::loadData(std::string fileloc){
    ifstream fin(fileloc);
    Matrix<string> data;
    fin >> data;
    fin.close();
    int rows = data.getRows();
    int cols = data.getCols();
    //Matrix<string> tempMat(rows-1, cols);

    vector<Attr> attrs;
    Matrix<int> matEncoded(rows-1,cols);
    for(int j=0; j<cols; ++j){
        vector<string> C;
        for(int i=1; i<rows; ++i){

            C.push_back(data[i][j]);
        }

        LabelEncoder<string> encoder;
        vector<int> ret = encoder.fitTransform(C);
        for(int i=1; i<rows;++i){
            matEncoded[i-1][j] = ret[i-1];
        }
        if(j<=cols-1){

            Attr ar;
            ar.attrName = data[0][j];
            ar.typeNum = j;

            set<int> vals;
            vector<string> attrV;
            unordered_map<string, int> table;
            for(int i=0; i<ret.size(); ++i){
                if(vals.count(ret[i])== 0){
                    vals.insert(ret[i]);
                    attrV.push_back(data[i+1][j]);
                    table[data[i+1][j]] = ret[i];
                }
            }
            ar.attrVals = attrV;
            ar.attrMap = table;
            attrs.push_back(ar);
        }
    }

    this->AttrVec = attrs;
    this->trainDataMat = matEncoded;
}

void DecisionTree::buildTree(const Matrix<int>& data, const std::vector<Attr>& dataAttr, string algo){
    if(algo == "ID3"){
        root = buildTreeWithID3(data,dataAttr);
    }
    /*else if(algo == "C4.5"){
        root = buildTreeWithC4_5(data,dataAttr);
    }
    else if(algo == "CART"){
        root = buildTreeWithCART(data,dataAttr);
    }*/
}

double DecisionTree::calcShannonEnt(vector<int>& vals){
    unordered_map<int,int> labelCounts;
    int numEntries = vals.size();

    double shannonEnt = 0.0;

    for(int i=0; i<numEntries; ++i){
        labelCounts[vals[i]]++;
    }
    for(auto it= labelCounts.begin(); it!= labelCounts.end(); ++it){

        double prob = double(it->second) / numEntries;
        shannonEnt -= prob * log2(prob);
    }
    return shannonEnt;
}

int DecisionTree::chooseBestFeatureToSplit(const Matrix<int>& data){
    int numFeatures = data.getCols()-1;
    int numEntries = data.getRows();

    vector<int> labels = data.getOneCol<vector<int>>(numFeatures-1);
    double baseEntropy = calcShannonEnt(labels);
    double bestInfoGain = 0.0; int bestFeature = -1;
    for(int i=0; i<numFeatures; ++i){
        unordered_map<int, vector<int>> table;
        for(int j=0; j<numEntries; ++j){
            table[data[j][i]].push_back(data[j][numFeatures]);
        }
        /*for(auto it = table.begin(); it!= table.end(); ++it){
            cout<<it->first<<endl;
            for(auto v:it->second)
                cout<<v<<" ";
            cout<<endl;
        }*/
        double newEntropy = 0.0;
        for(auto it = table.begin(); it!= table.end(); ++it){
            double prob = it->second.size() / numEntries;
            newEntropy += prob* calcShannonEnt(it->second);
        }

        double infoGain = baseEntropy - newEntropy;

        if(infoGain > bestInfoGain){
            bestInfoGain = infoGain;
            bestFeature = i;
        }
    }
    return bestFeature;
}

int DecisionTree::majorityCnt(vector<int>& labels){
    unordered_map<int,int> classCount;
    for(int i=0; i<labels.size(); ++i){
        classCount[labels[i]]++;
    }
    typedef function<bool(std::pair<int, int>, std::pair<int, int>)> Comparator;
    Comparator compFunctor =
            [](pair<int, int> elem1 ,pair<int, int> elem2)
            {
                return elem1.second > elem2.second;
            };
    set<pair<int, int>, Comparator> setOfClassCount(
            classCount.begin(), classCount.end(), compFunctor);
    auto it = setOfClassCount.begin();
    return (*it).first;
}

TreeNode* DecisionTree::buildTreeWithID3(const Matrix<int>& data, const vector<Attr>& dataAttr){
    int bestFeatureIdx = chooseBestFeatureToSplit(data);


    vector<int> labels = data.getOneCol<vector<int>>(data.getCols()-1);
    set<int> labelsSet = set<int>(labels.begin(),labels.end());
    if(labelsSet.size()==1){ //stop splitting when all of the classes are equal
        TreeNode* leafNode = new TreeNode();
        int label = labels[0];
        Attr atr = dataAttr[dataAttr.size()-1];
        string feat="";

        for(auto it = atr.attrMap.begin(); it!= atr.attrMap.end(); ++it){
            if(it->second == label) feat = it->first;
        }

        leafNode->setAttrName(feat);
        leafNode->setNodeType(true);
        return leafNode;

    } else if(data.getCols() == 2){//stop splitting when there are no more features in dataSet
        TreeNode* leafNode = new TreeNode();
        //int label = majorityCnt(labels);
        int label = labels[0];
        Attr atr = dataAttr[dataAttr.size()-1];
        string feat="";

        for(auto it = atr.attrMap.begin(); it!= atr.attrMap.end(); ++it){
            if(it->second == label) feat = it->first;
        }
        leafNode->setAttrName(feat);

        leafNode->setNodeType(true);
        return leafNode;
    }

    else{
        TreeNode* branchNode = new TreeNode();
        branchNode->setAttrName(dataAttr[bestFeatureIdx].attrName);
        branchNode->setNodeType(false);
        vector<Attr> tmpAttr = dataAttr;
        tmpAttr.erase(tmpAttr.begin() + bestFeatureIdx);

        vector<string> attrVs = dataAttr[bestFeatureIdx].attrVals;

        for(int i=0; i<attrVs.size(); ++i){
            TreeNode* child;
            string name = attrVs[i];

            auto valIt = dataAttr[bestFeatureIdx].attrMap.find(name);
            assert(valIt != dataAttr[bestFeatureIdx].attrMap.end());
            if(noEmpty(data, bestFeatureIdx, valIt->second)){
                Matrix<int> subMat;

                subMat = getSubMat(data, dataAttr, bestFeatureIdx, name);

                child = this->buildTreeWithID3(subMat, tmpAttr);

                branchNode->addChild(name, child);

            }
            else{
                TreeNode* leafNode = new TreeNode();
                int label = labels[0];
                Attr atr = dataAttr[dataAttr.size()-1];
                string feat="";

                for(auto it = atr.attrMap.begin(); it!= atr.attrMap.end(); ++it){
                    if(it->second == label) feat = it->first;
                }
                leafNode->setAttrName(feat);

                leafNode->setNodeType(true);
                return leafNode;
                //branchNode->addChild(name,leafNode);
            }
        }
        return branchNode;
    }

}

Matrix<int> DecisionTree::getSubMat(const Matrix<int>& data, const vector<Attr>& dataAttr, int idx,
        string attrName){

    int rows = data.getRows();
    int cols = data.getCols();
    auto attrValIt = dataAttr[idx].attrMap.find(attrName);
    assert(attrValIt != dataAttr[idx].attrMap.end());
    int attrVal = attrValIt->second;
    int c = 0;
    for(int i=0; i<rows; ++i){
        if(data[i][idx] == attrVal)
            c++;
    }
    Matrix<int> subMat(c,cols-1);
    c = 0;
    for(int i=0; i<rows; ++i){
        if(data[i][idx] == attrVal){
            //MyVector<int> vals;
            for(int j=0; j<idx; ++j){
                //vals.push_back(data[i][j]);
                subMat[c][j] = data[i][j];
            }
            for(int j=idx+1; j<cols; ++j){
                //vals.push_back(data[i][j]);
                subMat[c][j-1] = data[i][j];
            }
            c++;
            //subMat.push_back(vals);
            /*for(int i=0; i<subMat.getRows(); ++i){
                for(int j=0; j<subMat.getCols(); ++j){
                    cout<<subMat[i][j]<<" ";
                }
                cout<<endl;
            }*/
        }
    }
    return subMat;
}

bool DecisionTree::noEmpty(const Matrix<int>& data, int idx, int val){
    for(int i=0; i<data.getRows(); ++i){
        if(data[i][idx] == val) return true;
    }
    return false;
}

vector<string> DecisionTree::predict(vector<vector<string>>& data){
    vector<string> res;
    int rs = data.size();

    for(int i=0; i<rs; ++i){

        string ans = predictHelp(this->root, data[i], this->AttrVec);
        res.push_back(ans);
    }

    return res;
}

string DecisionTree::predictHelp(TreeNode* node, vector<string>& featureVals,
                        vector<Attr> attrV){

    if(node->getNodeType()){
        return node->getAttrName();
    }
    else{
        string nodeAttr = node->getAttrName();

        int attrIdx = -1;
        for(int i=0; i<attrV.size(); ++i){
            if(nodeAttr == attrV[i].attrName) attrIdx = i;
        }

        if(attrIdx >=0){
            string attr = featureVals[attrIdx];
            TreeNode* next = node->getChild(attr);
            return predictHelp(next, featureVals, attrV);
        }
    }
    return nullptr;
}

