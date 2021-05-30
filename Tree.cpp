//
// Created by xian on 25/05/2021.
//

#include "Tree.h"

Tree::Tree(){
    root= new Node('S');
}

Tree::~Tree(){
    clearRecursive(root);
}
bool Tree::insertChild(Node * actualNode, char &value,int index,int height, const bool &isfinal_)
{
    bool isTaller = false;
    Node * newNode = new Node(value,isfinal_,index,height, actualNode);
    actualNode->childs.push_back(newNode);

    return true;
}
void Tree::insertChilds(Node * node, char valueFather,int height,string newchilds){


        int index=0;
        node->value=valueFather;
        for (int i = 0; i < newchilds.size(); ++i) {
                if (!isupper(newchilds[i])){
                    insertChild(node,newchilds[i],0,height,true);

                }else{
                    insertChild(node,newchilds[i],index,height,false);
                    index++;
                }
        }

}

string Tree::getWord(){
    static string word ;
    static bool stop;
    word="";
    stop=false;
    recursiveGetWord(root,word,stop);
    return word;
}
bool Tree::recursiveGetWord(Node* node, string & word,bool & stop ){

    if (node->isFinal && !stop){
        word += node->value;
    }
    if (!node->isFinal && node->childs.size()==0){
        stop=true;
    }
    for (int i = 0; i < node->childs.size(); ++i) {
        recursiveGetWord(node->childs[i],word,stop);
    }


}
Node* Tree::getNextNonTerminal(Node * actualNode){

    static Node * node;
    node= nullptr;
    static bool stop;
    stop=false;
    recursiveGetNextNonTerminal(root,stop,node);

    return node;

}
 void Tree::recursiveGetNextNonTerminal(Node * actualNode,bool & stop,Node *& find){
    if (actualNode->childs.size()==0 && !actualNode->isFinal){
        if (!stop){
            find = actualNode;
            stop=true;
        }
    }else{
        for (int i = 0; i < actualNode->childs.size(); ++i) {
            recursiveGetNextNonTerminal(actualNode->childs[i],stop,find);
        }
    }




}
bool Tree::deleteChilds(Node * node){

    for (int i = 0; i < node->childs.size(); ++i) {
        clearRecursive(node->childs[i]);
    }
    node->childs.clear();



}
// *& IMPORTANTE
void Tree::clearRecursive(Node *& tree)
{
    if (tree != nullptr)
    {

        for (int i = 0; i < tree->childs.size(); ++i) {
            clearRecursive(tree->childs[i]);
        }

        delete tree;
        tree = nullptr;
    }
}

void Tree::insertChilds(Node *node, char valueFather, int height, string newchilds, int startingIndex) {

    node->value=valueFather;
    for (int i = 0; i < newchilds.size(); ++i) {
        if (!isupper(newchilds[i])){
            insertChild(node,newchilds[i],0,height,true);

        }else{
            insertChild(node,newchilds[i],startingIndex,height,false);
            startingIndex++;
        }
    }
}
