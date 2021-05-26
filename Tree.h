//
// Created by xian on 25/05/2021.
//

#ifndef POJECT_TREE_H
#define POJECT_TREE_H

#include <fstream>
#include <string>
#include <iostream>
#include <vector>

using namespace std;

class Node {


public:
    char value;
    bool isFinal;
    vector<Node*> childs;
    int height;
    int index;
    Node* father;
    Node() {
        value = NULL;
        isFinal=false;
        father= nullptr;
        index=0;

    }

    Node(char value_) {
        value = value_;
        isFinal=false;
        father= nullptr;
        index=0;

    }
    Node(char value_, bool isfinal_,int index_) {
        value = value_;
        isFinal=isfinal_;
        father= nullptr;
        index=index_;

    }
    Node(char value_, bool isfinal_,int index_,Node * father_) {
        value = value_;
        isFinal=isfinal_;
        father= father_;
        index=index_;

    }

};

class Tree {
public:

    Node * root;

    Tree();
    ~Tree();
    bool insertChild(Node * actualNode, char &value,int index, const bool &isfinal_);
    void insertChilds(char valueFather,string newchilds,vector<int> index);
    string getWord();
    bool recursiveGetWord(Node * node ,string & word,bool & stop);
    bool deleteChilds(Node * node);
    Node * getNextNonTerminal(Node * actualNode);
    void recursiveGetNextNonTerminal(Node * actualNode,bool & stop,Node *& find);
    void clearRecursive(Node *& tree);
private:

};


#endif //PROJECT_TREE_H
