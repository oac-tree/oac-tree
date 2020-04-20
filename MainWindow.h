#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QMenu>
#include <QToolBar>

class QTreeView;
class QStandardItemModel;

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
    QStandardItemModel *__model;
};

#endif // MAINWINDOW_H
