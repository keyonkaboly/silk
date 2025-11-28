#ifndef BROWSERFRAME_HPP
#define BROWSERFRAME_HPP

#include <QMainWindow>
#include <QTabWidget>
#include <QPushButton>
#include <QShortcut>
#include <QKeySequence>

class QKeyEvent;
class QResizeEvent;
class QObject;
class QEvent;

#include "defaulttab.hpp"

class browserframe : public QMainWindow{
    Q_OBJECT

public: 
    browserframe(QWidget *parent = nullptr);

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    bool eventFilter(QObject *obj, QEvent *event) override;

private slots:
    void addNewTab();
    void removeTab(int index);
    void closeAllTabs();
    void onTabChanged(int index);

private:
    QTabWidget *m_tabWidget;
    QPushButton *m_addTabButton;
    void setupTabs();
    void updateAddButtonPosition();
    void setupTabCloseButton(int index);
};

#endif 