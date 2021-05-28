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
    bool exist= runAlgorithm(tree.root,0,0,index,maxIndex);
    if (exist){
        ui->pushButton->setText("FUNCIONA");
    }

}

bool MainWindow::runAlgorithm(Node* actualnode, int indexWord, int height, vector<int> &index, vector<int> &maxIndex) {

    bool done= false;
    if (!done) {
        drawTree();

        if (height < 0) {
            return false;
        }else {

            if (index.size() <= height) {
                index.push_back(0);

                map<char, vector<string>>::iterator it = productions.find(actualnode->value);
                if (it != productions.end() && done != true) { //check
                    cout << "EXISTE2";
                    maxIndex.push_back(it->second.size() - 1);
                    done = runAlgorithm(actualnode, indexWord, height, index, maxIndex);

                } else {
                    Node *node = actualnode;
                    if (height != 0) {
                        index.pop_back();
                        maxIndex.pop_back();
                        index[height - 1]++;
                        //TODO borrar arbol¿?¿?
                        node = actualnode->father;
                        tree.deleteChilds(actualnode->father);
                    }
                    done = runAlgorithm(node, indexWord, height - 1, index, maxIndex);


                }

            } else {
                if (index[height] > maxIndex[height]) {
                    //not more options
                    Node *node = actualnode;
                    if (height != 0) {
                        index.pop_back();
                        maxIndex.pop_back();
                        index[height - 1]++;
                        //TODO borrar arbol¿?¿?
                        node = actualnode->father;
                        tree.deleteChilds(actualnode->father);
                    }
                    done = runAlgorithm(node, indexWord, height - 1, index, maxIndex);

                } else {


                    map<char, vector<string>>::iterator it = productions.find(actualnode->value);
                    if (it != productions.end()) {
                        cout << "EXISTE";


                        tree.insertChilds(actualnode, it->first, it->second[index[height]], index);
                        bool flagGoingGood = false;


                        //cambiar

                        if (!done && tree.getWord() == word.substr(0, tree.getWord().size())) {
                            flagGoingGood = true;
                        }


                        if (flagGoingGood) {
                            Node *node = tree.getNextNonTerminal(actualnode);
                            if (node != nullptr) {
                                cout << "DISTINTO A NULL" << endl;
                                actualnode = node;
                                done = runAlgorithm(actualnode, indexWord, height + 1, index, maxIndex);
                                //TOOD comprobar

                            } else {
                                if (tree.getWord() == word) {
                                    cout << "FINALIZADO";
                                    return true; //check to step out here directly

                                } else {
                                    index[height]++;
                                    tree.deleteChilds(actualnode);
                                    done = runAlgorithm(actualnode, indexWord, height, index, maxIndex);


                                }

                            }
                        } else {
                            //comprobar
                            index[height]++;
                            tree.deleteChilds(actualnode);

                            done = runAlgorithm(actualnode, indexWord, height, index, maxIndex);
                        }

                    } else {
                        cout << "NOEXISTE";
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
    addTreeRoot("A");
    addTreeRoot("B");
    addTreeRoot("C");


}
void MainWindow::addTreeRoot(QString name)
{
    // QTreeWidgetItem(QTreeWidget * parent, int type = Type)
    QTreeWidgetItem *treeItem = new QTreeWidgetItem(ui->treeWidget);

    // QTreeWidgetItem::setText(int column, const QString & text)
    treeItem->setText(0, name);
    addTreeChild(treeItem, name + "A");
    addTreeChild(treeItem, name + "B");
}

void MainWindow::addTreeChild(QTreeWidgetItem *parent, QString name)
{
    // QTreeWidgetItem(QTreeWidget * parent, int type = Type)
    QTreeWidgetItem *treeItem = new QTreeWidgetItem();

    // QTreeWidgetItem::setText(int column, const QString & text)
    treeItem->setText(0, name);


    // QTreeWidgetItem::addChild(QTreeWidgetItem * child)
    parent->addChild(treeItem);
}
