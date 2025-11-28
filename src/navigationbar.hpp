#ifndef NAVIGATIONBAR_HPP
#define NAVIGATIONBAR_HPP

#include <QWidget> // base class for all UI widgets
#include <QLineEdit> // for URL bar 
#include <QToolBar> // for back, forward, reload buttons (UI, frontend)
#include <QAction> // for actions like back, forward, reload (action backend)

class navigationbar : public QWidget { // navigationbar class inherits from QWidget
    Q_OBJECT // qt macro (rule) that must be there to execude code

public:
    navigationbar(QWidget *parent = nullptr);

signals: // qt specific access specifier that declares signals classes can emit
    // signals are messages emitted by objects when an internal state changes
    // which can be onnected to a slot (a special function) to trigger an action
    // ex of a signal, closing a tab, reload clicked, etc.

    void navigateToUrl(const QString &url);
    void goBackClicked();
    void goForwardClicked();
    void reloadClicked();

private: // just got a bunch of diff pointers in here private to their own classes
    QLineEdit *urlBar;
    QToolBar *toolBar;
    QAction *backAction;
    QAction *forwardAction;
    QAction *reloadAction;
};

#endif
