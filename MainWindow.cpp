#include "MainWindow.h"

#include <QMenuBar>
#include <QTreeView>
#include <QStandardItemModel>
#include <QStandardItem>

QList<QStandardItem *> PrepareRow(const QString &first,
                                  const QString &second,
                                  const QString &third)
{
    return { new QStandardItem(first),
             new QStandardItem(second),
             new QStandardItem(third) };
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , __treeview(new QTreeView(this))
    , __model(new QStandardItemModel(this))
{
    setWindowTitle("CODAC Sequencer");
    setCentralWidget(__treeview);

    QList<QStandardItem *> preparedRow = PrepareRow("first", "second", "third");
    QStandardItem *item = __model->invisibleRootItem();
    item->appendRow(preparedRow);

    QList<QStandardItem *> secondRow = PrepareRow("1", "2", "3");
    preparedRow.first()->appendRow(secondRow);

    __treeview->setModel(__model);
    __treeview->expandAll();

    CreateMenu();
    CreateToolbar();
}

void MainWindow::CreateMenu()
{
    __file_menu = menuBar()->addMenu("&File");
    __file_menu->addAction("&Exit", this, &MainWindow::close);
}

void MainWindow::CreateToolbar()
{
    __toolbar = new QToolBar();
    addToolBar(__toolbar);
    __toolbar->addAction("Exit", this, &MainWindow::close);
}
