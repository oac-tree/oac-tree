#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QMenu>
#include <QToolBar>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

private:
    void CreateMenu();
    void CreateToolbar();
    QLabel *m_label;
    QMenu *m_file_menu;
    QToolBar *m_toolbar;
};

#endif // MAINWINDOW_H
