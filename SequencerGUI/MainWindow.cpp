#include "MainWindow.h"

#include "TestModel.h"
#include "viewmodel/ViewModel.h"

#include <QMenuBar>
#include <QTreeView>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , __treeview(new QTreeView(this))
    , __model(Sequencer::TestModel::CreateTestModel())
    , __view_model(new Sequencer::ViewModel::TreeViewModel(__model))
{
    setWindowTitle("CODAC Sequencer");
    setCentralWidget(__treeview);

    __treeview->setModel(__view_model);
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
