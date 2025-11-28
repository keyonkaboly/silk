#ifndef DEFAULTTAB_HPP
#define DEFAULTTAB_HPP

#include <QWidget>
#include <QWebEngineView>

class defaulttab : public QWidget
{
    Q_OBJECT

public:
    // constructor for defaulttab class
    // explicit
    explicit defaulttab(QWidget *parent = nullptr);

    // getter method gives other objects access to web view
    // const bc it doesnt modify the class
    QWebEngineView *webView() const;

signals:
    // the diff signals emitted
    // webpage title changes, url changes
    void titleChanged(const QString &title);
    void urlChanged(const QUrl &url);

private:
    // pointer to the web view that disaplys the actual web page
    QWebEngineView *m_webView;
};

#endif