#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QMenu>
#include <QToolBar>

#include "viewmodel/ViewModel.h"
#include "model/Item.h"

class QTreeView;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private:
    void CreateMenu();
    void CreateToolbar();
    QMenu *__file_menu;
    QToolBar *__toolbar;
    QTreeView *__treeview;
    Sequencer::Model::Item *__model;
    Sequencer::ViewModel::TreeViewModel * __view_model;
};

#endif // MAINWINDOW_H
