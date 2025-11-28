#ifndef DEFAULTTAB_HPP
#define DEFAULTTAB_HPP

#include <QWidget>
#include <QWebEngineView>

class defaulttab : public QWidget
{
    Q_OBJECT

public:
    explicit defaulttab(QWidget *parent = nullptr);
    QWebEngineView *webView() const;

signals:
    void titleChanged(const QString &title);
    void urlChanged(const QUrl &url);

private:
    QWebEngineView *m_webView;
};

#endif