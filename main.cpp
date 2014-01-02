#include <QtGui/QApplication>
#include <QtOpenGL/QGLWidget>
#include <QDesktopWidget>
#include "GLWidget.h"
#include "Point.h"
#include <set>
#include <iostream>

using namespace std;
using Nearest::Point;




int main(int argc, char *argv[]){
        QApplication app(argc, argv);
        GLWidget window;
        int windowWidth = 800;
        int windowHeight = 600;
        window.resize(windowWidth,windowHeight);
        QRect screen = app.desktop()->screenGeometry();
        window.move(screen.width()/2 - windowWidth/2,
                    screen.height()/2 - windowHeight/2);
        window.show();

        return app.exec();

        return 0;
}
