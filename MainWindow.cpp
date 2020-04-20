#include "MainWindow.h"

#include <QMenuBar>

MainWindow::MainWindow()
    : QMainWindow()
{
    setWindowTitle("CODAC Sequencer");
    m_label = new QLabel(this);
    m_label->setText("<h1>Sequencer main area</h1>");
    setCentralWidget(m_label);
    CreateMenu();
    CreateToolbar();
}

void MainWindow::CreateMenu()
{
    m_file_menu = menuBar()->addMenu("&File");
    m_file_menu->addAction("&Exit", this, &MainWindow::close);
}

void MainWindow::CreateToolbar()
{
    m_toolbar = new QToolBar();
    addToolBar(m_toolbar);
    m_toolbar->addAction("Exit", this, &MainWindow::close);
}
