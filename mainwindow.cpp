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


    word = this->ui->lineEdit->text().toStdString();

    ui->label->setText("");


    static vector<vector<int>> index;
    static vector<vector<int>> maxIndex;
    static vector<int> indexNonTerminal;
    static int height;
    height =0;
    indexNonTerminal.clear();
    index.clear();
    maxIndex.clear();


    bool exist;
    if (word=="") {
        ui->label->setText("PLEASE enter a word");
        QPalette palette = ui->label->palette();
        palette.setColor(ui->label->foregroundRole(), Qt::red);
        ui->label->setPalette(palette);

    }else if (word.find('e') != std::string::npos) {

        ui->label->setText("The word shouldn't contain empty symbol e");
        QPalette palette = ui->label->palette();
        palette.setColor(ui->label->foregroundRole(), Qt::red);
        ui->label->setPalette(palette);
    }else{

         exist= runAlgorithm(tree.root,indexNonTerminal,height,index,maxIndex);

        if (exist){
            ui->label->setText("TRUE, The word is correct for this grammar.");
            QPalette palette = ui->label->palette();
            palette.setColor(ui->label->foregroundRole(), Qt::green);
            ui->label->setPalette(palette);
        }else{
            ui->label->setText("FALSE, The word is not correct for this grammar.");
            QPalette palette = ui->label->palette();
            palette.setColor(ui->label->foregroundRole(), Qt::red);
            ui->label->setPalette(palette);
        }

    }





}

bool MainWindow::runAlgorithm(Node* actualnode, vector<int> indexNonTerminal, int & height, vector<vector<int>> &index, vector<vector<int>> &maxIndex ) {

    bool done= false;
    if (!done) {


        drawTree();
        sleep(1);

        if (height < 0) {

            return false;
        }else {

            if (index.size() <= height) {
                vector<int> aux;
                index.push_back(aux);
                maxIndex.push_back(aux);
                bool hasNonTerminal=false;
                if (height==0){
                    map<char, vector<string>>::iterator it = productions.find(actualnode->value);
                    if (it != productions.end() && done != true) { //check

                        maxIndex[height].push_back(it->second.size() - 1);
                        index[height].push_back(0);
                        hasNonTerminal=true;

                    }
                }else {
                    for (int i = 0; i < actualnode->father->childs.size(); ++i) {
                        if (!actualnode->father->childs[i]->isFinal) {

                            map<char, vector<string>>::iterator it = productions.find( actualnode->father->childs[i]->value);
                            if (it != productions.end() && done != true) { //check
                                maxIndex[height].push_back(it->second.size() - 1);
                                index[height].push_back(0);
                                hasNonTerminal = true;
                            }


                        }

                    }
                }
                if (hasNonTerminal){
                    indexNonTerminal.push_back(0);
                    done = runAlgorithm(actualnode, indexNonTerminal, height, index, maxIndex);
                }else{
                    Node *node = actualnode;
                    if (height != 0) {
                        index.pop_back();
                        maxIndex.pop_back();
                        index[height - 1][indexNonTerminal[height-1]]++;
                        node = actualnode->father;
                        tree.deleteChilds(actualnode->father);
                    }
                    height--;
                    done = runAlgorithm(node, indexNonTerminal, height, index, maxIndex);

                }


            } else {
                if (index[height][indexNonTerminal[height]] > maxIndex[height][indexNonTerminal[height]]) {

                    Node *node = actualnode;
                    if (height != 0) {
                        index.pop_back();
                        maxIndex.pop_back();
                        index[height - 1][indexNonTerminal[height-1]]++;
                        node = actualnode->father;
                        tree.deleteChilds(actualnode->father);
                    }else{
                        return false;
                    }
                    height--;
                    done = runAlgorithm(node, indexNonTerminal, height, index, maxIndex);

                } else {


                    map<char, vector<string>>::iterator it = productions.find(actualnode->value);
                    if (it != productions.end()) {

                        if (index.size() > height+1){
                            tree.insertChilds(actualnode, it->first,height+1, it->second[index[height][indexNonTerminal[height]]],index[height+1].size(),index[height][indexNonTerminal[height]]);

                        }else{
                            tree.insertChilds(actualnode, it->first,height+1, it->second[index[height][indexNonTerminal[height]]],index[height][indexNonTerminal[height]]);

                        }

                        bool flagGoingGood = false;


                        if (!done && tree.getWord() == word.substr(0, tree.getWord().size())) {
                            flagGoingGood = true;
                        }


                        if (flagGoingGood) {

                            Node *node = tree.getNextNonTerminal(actualnode);
                            if (node != nullptr) {
                                actualnode = node;
                                indexNonTerminal[actualnode->height]= actualnode->index;
                                height=actualnode->height;
                                if (indexNonTerminal[actualnode->height]> index[actualnode->height].size()-1){
                                    map<char, vector<string>>::iterator it = productions.find(actualnode->value);
                                    if (it != productions.end() && done != true) { //check

                                        maxIndex[height].push_back(it->second.size() - 1);
                                        index[height].push_back(0);
                                    }



                                }

                                done = runAlgorithm(actualnode, indexNonTerminal, height, index, maxIndex);


                            } else {
                                if (tree.getWord() == word) {
                                    cout << "FINISH";
                                    drawTree();
                                    return true; //check to step out here directly

                                } else {
                                    index[height][indexNonTerminal[height]]++;
                                    tree.deleteChilds(actualnode);
                                    done = runAlgorithm(actualnode, indexNonTerminal, height, index, maxIndex);


                                }

                            }
                        } else {

                            index[height][indexNonTerminal[height]]++;
                            tree.deleteChilds(actualnode);

                            done = runAlgorithm(actualnode, indexNonTerminal, height, index, maxIndex);
                        }

                    } else {
                        drawTree();
                        return false; //check to step out here directly
                    }


                }
            }
        }
    }
    return done;
}

void MainWindow::drawTree() {

    ui->treeWidget->clear();
    // Set the number of columns in the tree
    ui->treeWidget->setColumnCount(1);


    // Add root nodes
    QTreeWidgetItem *treeItem = new QTreeWidgetItem(ui->treeWidget);

    treeItem->setExpanded(true);
    // QTreeWidgetItem::setText(int column, const QString & text)
    QString str= QChar(tree.root->value);
    str+= '/';
    str+= QString::number(tree.root->indexNonTerminal);
    treeItem->setText(0, str);
    for (int i = 0; i < tree.root->childs.size(); ++i) {
        addTreeChild(treeItem, tree.root->childs[i]);
    }

    ui->treeWidget->expandAll();
    qApp->processEvents();

}

void MainWindow::addTreeChild(QTreeWidgetItem *parent, Node * node)
{
    QTreeWidgetItem *treeItem = new QTreeWidgetItem();


    QString str= QChar(node->value);
    if (!node->isFinal){
        str+= '/';
        str+= QString::number(node->indexNonTerminal);
    }
    treeItem->setText(0, str);


    for (int i = 0; i < node->childs.size(); ++i) {
        addTreeChild(treeItem, node->childs[i]);
    }
    treeItem->setExpanded(true);
    parent->addChild(treeItem);




}
