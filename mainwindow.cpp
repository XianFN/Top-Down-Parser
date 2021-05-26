//
// Created by xian on 25/05/2021.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MainWindow.h" resolved


#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace std;


MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    OpenQFile();


}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::OpenQFile() {
    QString url = QFileDialog::getOpenFileName(this, "Select the Grammar", "/home/");
    ifstream myReadFile;
    myReadFile.open(url.toStdString());
    char output[100];
    if (myReadFile.is_open()) {
        while (!myReadFile.eof()) {
            string s;
            getline(myReadFile, s);

            string delimiter = " -> ";

            size_t pos = 0;
            char key;
            pos = s.find(delimiter);
            key = s[0];
            s.erase(0, pos + delimiter.length());


            vector<string> productionsVector;
            delimiter = " | ";

            pos = 0;
            string productionString;
            while ((pos = s.find(delimiter)) != std::string::npos) {
                productionString = s.substr(0, pos);
                productionsVector.push_back({productionString});
                s.erase(0, pos + delimiter.length());
            }
            productionsVector.push_back({s});
            productions.insert({key, productionsVector});


        }
    }
    myReadFile.close();
}

void MainWindow::on_pushButton_clicked() {


//    word = this->ui->lineEdit->text().toStdString();
    word= "aacbc";
    static vector<int> index;
    static vector<int> maxIndex;
    runAlgorithm(tree.root,0,0, 0, index,maxIndex);

}

void MainWindow::runAlgorithm(Node* actualnode,int indexWord,int height, int col, vector<int> &index, vector<int> &maxIndex) {



    if (height<0){
        //ACABA
    }

    if (index.size()<= height){
        index.push_back(0);

        map<char,vector<string>>::iterator it= productions.find(actualnode->value);
        if (it!=productions.end() ){
            cout<<"EXISTE";
            maxIndex.push_back(it->second.size());
            runAlgorithm(actualnode,indexWord,height,col,index,maxIndex);
        }else{
            cout<<"NOEXISTE";
        }

    }else {
        if (index[height] > maxIndex[height]){
            //not more options
            if (height!=0) {
                index.pop_back();
                maxIndex.pop_back();
                index[height - 1]++;
                //TODO borrar arbol¿?¿?
                tree.deleteChilds(actualnode->father);
            }
                runAlgorithm(actualnode, indexWord, height - 1, col, index, maxIndex);

        }else{

            map<char,vector<string>>::iterator it= productions.find(actualnode->value);
            if (it!=productions.end() ){
                cout<<"EXISTE";
                /*inutil
                for (int i = index[height]; i < it->second.size(); ++i) { // < or <= ?¿
                    it++;
                }
                */

                tree.insertChilds(it->first, it->second[index[height]], index);
                bool flagGoingGood = false;


                //cambiar
                string actualWord = tree.getWord();
                if (actualWord == word.substr(0,actualWord.size() )) {
                        flagGoingGood = true;
                }



                if (flagGoingGood) {
                    Node *node = tree.getNextNonTerminal(actualnode);
                    if (node != nullptr) {
                        cout << "DISTINTO A NULL" << endl;
                        runAlgorithm(node,indexWord,height+1,col,index,maxIndex);

                    } else {
                        if (tree.getWord() == word) {
                            cout << "FINALIZADO";
                        } else {
                            if (height!=0){
                                index[height-1]++;
                                runAlgorithm(node,indexWord,height-1,col,index,maxIndex);

                            }else{
                                runAlgorithm(node,indexWord,height-1,col,index,maxIndex);
                                //FINALIZA
                            }


                        }
                        runAlgorithm(node,indexWord,height+1,col,index,maxIndex);
                    }
                }else{
                    //comprobar
                    index[height]++;
                    tree.deleteChilds(actualnode);

                    runAlgorithm(actualnode,indexWord,height,col,index,maxIndex);

                }


            }else{
                cout<<"NOEXISTE";
            }



        }

    }

}
