#include "navigationbar.hpp"
//will include refresh, back button, forward button, url bar.

navigationbar::navigationbar(QWidget *parent) : QWidget(parent)
{
    urlBar = new QLineEdit(this);
    toolBar = new QToolBar(this);

    backAction = new QAction("Back", this);
    forwardAction = new QAction("Forward", this);
    reloadAction = new QAction("Reload", this);

    toolBar->addAction(backAction);
    toolBar->addAction(forwardAction);
    toolBar->addAction(reloadAction);
    toolBar->addWidget(urlBar);
}
