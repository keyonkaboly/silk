#include "browserframe.hpp"
#include <QTabBar>
#include <QTimer>
#include <QKeyEvent>
#include <QShortcut>
#include <QApplication>
#include <QResizeEvent>
#include <QEvent>
#include <QWebEngineView>
#include <QEventLoop>

browserframe::browserframe(QWidget *parent) : QMainWindow(parent)
{
    setupTabs();
    
    resize(2000, 1400);
    setWindowTitle("silk");
}

void browserframe::setupTabs()
{
    m_tabWidget = new QTabWidget(this);
    m_tabWidget->setTabsClosable(false); // We'll use custom close buttons
    m_tabWidget->setMovable(true);
    
    // Get the tab bar
    QTabBar *tabBar = m_tabWidget->tabBar();
    
    // Style the tab bar to remove default close button styling
    tabBar->setStyleSheet(
        "QTabBar::close-button {"
        "    image: none;"
        "}"
    );
    
    // Connect signals
    connect(m_tabWidget, &QTabWidget::currentChanged, this, &browserframe::onTabChanged);
    connect(tabBar, &QTabBar::tabMoved, [this]() {
        updateAddButtonPosition();
        // Re-setup all close buttons after move
        for (int i = 0; i < m_tabWidget->count(); ++i) {
            setupTabCloseButton(i);
        }
    });
    
    // Add the "+" button as a widget that appears after tabs
    m_addTabButton = new QPushButton("+", tabBar);
    m_addTabButton->setFixedSize(30, tabBar->height());
    m_addTabButton->setToolTip("New Tab");
    m_addTabButton->setStyleSheet(
        "QPushButton {"
        "    border: none;"
        "    background: transparent;"
        "    font-size: 18px;"
        "    color: #666666;"
        "    padding: 0px;"
        "}"
        "QPushButton:hover {"
        "    color: #333333;"
        "    background: rgba(0, 0, 0, 0.1);"
        "    border-radius: 2px;"
        "}"
    );
    connect(m_addTabButton, &QPushButton::clicked, this, &browserframe::addNewTab);
    
    // Setup keyboard shortcuts - set context to WidgetShortcut so they work even when web view has focus
    QShortcut *newTabShortcut = new QShortcut(QKeySequence("Ctrl+T"), this);
    newTabShortcut->setContext(Qt::WidgetShortcut);
    connect(newTabShortcut, &QShortcut::activated, this, &browserframe::addNewTab);
    
    QShortcut *closeTabShortcut = new QShortcut(QKeySequence("Ctrl+W"), this);
    closeTabShortcut->setContext(Qt::WidgetShortcut);
    connect(closeTabShortcut, &QShortcut::activated, [this]() {
        int currentIndex = m_tabWidget->currentIndex();
        if (currentIndex >= 0) {
            removeTab(currentIndex);
        }
    });
    
    QShortcut *closeAllTabsShortcut = new QShortcut(QKeySequence("Ctrl+Shift+W"), this);
    closeAllTabsShortcut->setContext(Qt::WidgetShortcut);
    connect(closeAllTabsShortcut, &QShortcut::activated, this, &browserframe::closeAllTabs);
    
    // Create and add the first tab
    addNewTab();
    
    // Update button position after first tab is added
    QTimer::singleShot(0, [this]() { updateAddButtonPosition(); });
    
    setCentralWidget(m_tabWidget);
}

void browserframe::addNewTab()
{
    defaulttab *newTab = new defaulttab(this);
    int index = m_tabWidget->addTab(newTab, "New Tab");
    m_tabWidget->setCurrentIndex(index);
    
    // Install event filter on web view to catch keyboard shortcuts
    QWebEngineView *webView = newTab->webView();
    if (webView) {
        webView->installEventFilter(this);
    }
    
    // Setup the close button for this tab
    setupTabCloseButton(index);
    
    // Update tab title when page title changes
    connect(newTab, &defaulttab::titleChanged, [this, newTab](const QString &title) {
        int currentIndex = m_tabWidget->indexOf(newTab);
        if (currentIndex >= 0) {
            m_tabWidget->setTabText(currentIndex, title.isEmpty() ? "New Tab" : title);
        }
    });
    
    // Update add button position
    QTimer::singleShot(0, [this]() { updateAddButtonPosition(); });
}

void browserframe::removeTab(int index)
{
    if (m_tabWidget->count() > 1) {
        QWidget *tabWidget = m_tabWidget->widget(index);
        m_tabWidget->removeTab(index);
        delete tabWidget;
        
        // Update add button position after removal
        QTimer::singleShot(0, [this]() { 
            updateAddButtonPosition();
            // Re-setup close buttons for remaining tabs
            for (int i = 0; i < m_tabWidget->count(); ++i) {
                setupTabCloseButton(i);
            }
        });
    }
}

void browserframe::setupTabCloseButton(int index)
{
    QTabBar *tabBar = m_tabWidget->tabBar();
    
    // Remove existing button if any
    QWidget *existingButton = tabBar->tabButton(index, QTabBar::RightSide);
    if (existingButton) {
        existingButton->deleteLater();
    }
    
    // Create a custom close button with minus sign
    QPushButton *closeButton = new QPushButton(tabBar);
    closeButton->setText("−"); // Unicode minus sign (U+2212)
    closeButton->setFixedSize(24, 24);
    closeButton->setToolTip("Close Tab");
    closeButton->setFlat(true);
    closeButton->setStyleSheet(
        "QPushButton {"
        "    border: none;"
        "    background: transparent;"
        "    font-size: 20px;"
        "    font-weight: bold;"
        "    color: #999999;"
        "    padding: 2px;"
        "    text-align: center;"
        "    min-width: 24px;"
        "    min-height: 24px;"
        "}"
        "QPushButton:hover {"
        "    color: #333333;"
        "    background: rgba(0, 0, 0, 0.1);"
        "    border-radius: 3px;"
        "}"
        "QPushButton:pressed {"
        "    background: rgba(0, 0, 0, 0.2);"
        "}"
    );
    
    // Connect the button to remove the tab - use a lambda that finds the current index
    connect(closeButton, &QPushButton::clicked, [this, closeButton]() {
        QTabBar *tabBar = m_tabWidget->tabBar();
        // Find which tab this button belongs to
        for (int i = 0; i < tabBar->count(); ++i) {
            if (tabBar->tabButton(i, QTabBar::RightSide) == closeButton) {
                removeTab(i);
                break;
            }
        }
    });
    
    // Set the button on the tab
    tabBar->setTabButton(index, QTabBar::RightSide, closeButton);
}

void browserframe::updateAddButtonPosition()
{
    QTabBar *tabBar = m_tabWidget->tabBar();
    if (!tabBar || !m_addTabButton) return;
    
    // Calculate position after the last tab
    int lastTabIndex = tabBar->count() - 1;
    if (lastTabIndex >= 0) {
        QRect lastTabRect = tabBar->tabRect(lastTabIndex);
        // Position immediately after the tab, with minimal gap
        int x = lastTabRect.right() + 2;
        int y = (tabBar->height() - m_addTabButton->height()) / 2;
        
        // Make sure button is within tab bar bounds
        if (x + m_addTabButton->width() > tabBar->width()) {
            x = tabBar->width() - m_addTabButton->width() - 2;
        }
        
        m_addTabButton->move(x, y);
        m_addTabButton->raise(); // Ensure it's on top
    } else {
        // No tabs, position at start
        m_addTabButton->move(5, (tabBar->height() - m_addTabButton->height()) / 2);
    }
}

void browserframe::closeAllTabs()
{
    // Close all tabs except keep at least one
    while (m_tabWidget->count() > 1) {
        removeTab(0);
    }
}

void browserframe::onTabChanged(int index)
{
    // Update add button position when tab changes
    QTimer::singleShot(0, [this]() { updateAddButtonPosition(); });
    Q_UNUSED(index);
}

void browserframe::keyPressEvent(QKeyEvent *event)
{
    // Handle keyboard shortcuts
    if (event->modifiers() & Qt::ControlModifier) {
        if (event->key() == Qt::Key_T) {
            addNewTab();
            event->accept();
            return;
        } else if (event->key() == Qt::Key_W) {
            if (event->modifiers() & Qt::ShiftModifier) {
                closeAllTabs();
            } else {
                int currentIndex = m_tabWidget->currentIndex();
                if (currentIndex >= 0) {
                    removeTab(currentIndex);
                }
            }
            event->accept();
            return;
        }
    }
    QMainWindow::keyPressEvent(event);
}

void browserframe::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    // Update add button position when window is resized
    QTimer::singleShot(0, [this]() { updateAddButtonPosition(); });
}

bool browserframe::eventFilter(QObject *obj, QEvent *event)
{
    // Intercept keyboard events from web views to handle shortcuts
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        if (keyEvent->modifiers() & Qt::ControlModifier) {
            if (keyEvent->key() == Qt::Key_T) {
                addNewTab();
                return true;
            } else if (keyEvent->key() == Qt::Key_W) {
                if (keyEvent->modifiers() & Qt::ShiftModifier) {
                    closeAllTabs();
                } else {
                    int currentIndex = m_tabWidget->currentIndex();
                    if (currentIndex >= 0) {
                        removeTab(currentIndex);
                    }
                }
                return true;
            }
        }
    }
    return QMainWindow::eventFilter(obj, event);
}