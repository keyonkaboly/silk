#include <QApplication>
#include "browserframe.hpp"

// argc stands for argument count, num of strings passed from program to cmd line
// char *argv[] is equivalent to char* argv[]
// char* argv[] is an array of C-string pointers, helps read stuff passed in terminal 
// when running code
int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // creates browser frame "silk"
    browserframe browser;
    browser.show();
    
    return app.exec();
}