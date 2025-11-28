#include "defaulttab.hpp"
#include <QVBoxLayout>
#include <QUrl>

defaulttab::defaulttab(QWidget *parent) : QWidget(parent)
{
    // creates web browser view and sets the parent to "this"
    m_webView = new QWebEngineView(this);
    
    // loading the default tab page. For now it's google. Later on switch to silk site or smthn.
    m_webView->load(QUrl("https://www.google.com"));
    
    // creats a vertical box layout for the tab. "this" means it belongs to the tab.
    // sets margins to 0 so the webpage fills the tab flushly.
    // places web view inside the layout
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(m_webView);
    
    // connect signals for URL change and tab title change
    connect(m_webView, &QWebEngineView::titleChanged, this, &defaulttab::titleChanged);
    connect(m_webView, &QWebEngineView::urlChanged, this, &defaulttab::urlChanged);
}

// getter method gives other objects access to the web view so i can set up back, reload, zoom.
QWebEngineView *defaulttab::webView() const
{
    return m_webView;
}
