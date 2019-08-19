/*
 * DecicionTree.h
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


#ifndef MYML_DECICIONTREE_H
#define MYML_DECICIONTREE_H

#include <unordered_map>
#include <set>
#include <string>
#include <vector>
#include <cppMLKit/Matrix.h>
#include <fstream>
#include <math.h>
#include <cppMLKit/LabelEncoder.h>


class TreeNode{

    std::string attrName; //属性名
    //string type; //离散型、连续性
    bool leafNode; //是否是叶子节点
    std::unordered_map<std::string, TreeNode*> children;

public:
    TreeNode() {};
    virtual ~TreeNode() {
        for (auto &child : children) {
            if (child.second) {
                delete child.second;
                child.second = nullptr;
            }
        }
    }
    bool getNodeType() {return this->leafNode;}
    void setNodeType(bool flag){this->leafNode = flag;}
    TreeNode(std::string attr) : attrName(attr) {}
    std::string getAttrName(){
        return attrName;
    }
    void setAttrName(const std::string& attr_name){
        this->attrName = attr_name;
    }
    void addChild(std::string attrN, TreeNode *node) {
        children[attrN] = node;
    }
    TreeNode* getChild(std::string attrN);
    std::vector<TreeNode*> getChildrenPointers();

};

class DiscreteTreeNode: public TreeNode {

};

/*class Attr{
    int colIndex;
    std::string attrName;//属性名
    int typeNum;
    std::vector<std:: string> attrVals;
    std::unordered_map<std::string, int> table;
public:
    Attr() {};
    std:: string getAttrName(){return attrName;}
    void setAttrName(std::string name){ this->attrName = name;}
    int getTypeNum() { return typeNum;}
    void setTypeNum(int typeNum) {this->typeNum = typeNum;}
    std::vector<std::string> getAttrVals(){
        return this->attrVals;
    }
    void setAttrVals(std::vector<std::string> vals){
        this->attrVals = vals;
    }
    std::unordered_map<std::string, int> get
};*/

class DecisionTree{
public:
    struct Attr{
        int colIndex;
        std::string attrName;//属性名
        int typeNum;
        std::vector<std:: string> attrVals;
        std::unordered_map<std::string, int> attrMap;
    };

private:
    TreeNode* root;
    Matrix<int> trainDataMat;
    //std::vector<std::string> labelVec;
    std::vector<Attr> AttrVec;



public:
    DecisionTree() : root(nullptr) {};
    virtual ~DecisionTree() { if (root != nullptr) { delete root; }};

    void loadData(std::string fileloc);
    void buildTree(const Matrix<int>& data, const std::vector<Attr>& dataAttr,std::string algo = "ID3");
    Matrix<int> getTrainDataMat() {return this->trainDataMat;}
    std::vector<Attr> getAttrVec() {return this->AttrVec;}

    int chooseBestFeatureToSplit(const Matrix<int>& data);
    double calcShannonEnt(std::vector<int>& vals);

    bool noEmpty(const Matrix<int>& data, int idx, int val);

    std::vector<std::string> predict(std::vector<std::vector<std::string>>& data);

private:
    TreeNode* buildTreeWithID3(const Matrix<int>& dataMat, const std::vector<Attr>& dataAttr);
    TreeNode* buildTreeWithC4_5(const Matrix<int>& data, const std::vector<Attr>& dataAttr);
    TreeNode* buildTreeWithCART(const Matrix<int>& data, const std::vector<Attr>& dataAttr);
    Matrix<int> getSubMat(const Matrix<int>& data, const std::vector<Attr>& dataAttr, int idx,
                                        std::string attrName);
    int majorityCnt(std::vector<int>& labels);
    std::string predictHelp(TreeNode* node, std::vector<std::string>& featureVals,
            std::vector<Attr> attrV);
};
#endif //MYML_DECICIONTREE_H
