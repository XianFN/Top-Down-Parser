//
// Created by xian on 25/05/2021.
//

#ifndef POJECT_MAINWINDOW_H
#define POJECT_MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <QTreeWidget>
#include "Tree.h"
using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    QTreeWidget treeWidget;
    std::map<char,vector<string>> productions;
    string word;
    Tree tree;

    ~MainWindow() override;
    void drawTree();
    void OpenQFile();
    bool runAlgorithm(Node* actualnode, int indexWord, int height,vector<int> &index, vector<int> &maxIndex);
    void addTreeRoot(QString name);
    void addTreeChild(QTreeWidgetItem *parent, QString name);
    private slots:
    void on_pushButton_clicked();
private:
    Ui::MainWindow *ui;
};


#endif //POJECT_MAINWINDOW_H
