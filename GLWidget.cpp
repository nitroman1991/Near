#include <QtGui/QMouseEvent>
#include <QFont>
#include <QTextStream>
#include <QMessageBox>
#include <iostream>
#include <fstream>
#include <assert.h>
#include <map>

#include "near.h"
#include "Point.h"
#include "GLWidget.h"

using namespace std;
using namespace Nearest;



int windowWidth;
int windowHeight;
int worldWidth = 50;
int worldHeight;
Point worldPosition(0,0);
Point mousePosition(0,0);

std::vector<double> keys;
std::map<double, Nearest::Point> points;
std::vector<std::pair<Nearest::Point, Nearest::Point> > result;
bool drawHelp = true;
bool new_draw = true;
QString clear;
QString deletePoint;
QString space;
QString enter;
QString save;
QString open;
QString movement;
QString zoom;
QString help;
QString sExit;
QString res;



GLWidget::GLWidget(QWidget *parent) : QGLWidget(parent) {
    setMouseTracking(true);
    QTextStream(&deletePoint) << "D: delete last point";
    QTextStream(&space) << "Space: find pearest pair";
    QTextStream(&clear) << "C: clear all points";
    QTextStream(&save) << "S: save data to test.txt";
    QTextStream(&open) << "O: create data from test.txt";
    QTextStream(&movement) << "Arrows: movement";
    QTextStream(&zoom) << "+\\-: zoom";
    QTextStream(&sExit) << "Esc: exit";
    QTextStream(&help) << "F1: enable\\disable draw help";

}

void GLWidget::initializeGL() {
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_COLOR_MATERIAL);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0, 0, 0, 0);
}

void GLWidget::resizeGL(int w, int h) {
    windowWidth = w;
    windowHeight = h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    worldHeight = ((double)h/w)*worldWidth;
    glOrtho(worldPosition.x, worldPosition.x + worldWidth,
            worldPosition.y, worldPosition.y + worldHeight,
            -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void GLWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT);

    glColor4f(1,1,1,0.1);
    glBegin(GL_LINES);
    for (int i = 0; i < worldHeight; i++) {
        glVertex2d(worldPosition.x, worldPosition.y + i);
        glVertex2d(worldPosition.x + worldWidth, worldPosition.y + i);
    }
    for (int i = 0; i < worldWidth; i++) {
        glVertex2d(worldPosition.x + i, worldPosition.y + 0);
        glVertex2d(worldPosition.x + i, worldPosition.y + worldHeight);
    }
    glEnd();

    glBegin(GL_LINES);
    glVertex2d(0, -50);
    glVertex2d(0, 50);
    glVertex2d(-50, 0);
    glVertex2d(50, 0);
    glEnd();


    glColor4f(1,0,0,1);
    glPointSize(5);

    glBegin(GL_POINTS);
    for (std::map<double, Point>::iterator i = points.begin(); i != points.end(); ++i)
        glVertex2d(i -> second.x, i -> second.y);
    glEnd();

    glColor3f(1, 1, 0);
    if(!result.empty())
    {
        glBegin(GL_LINES);
        glVertex2d(result.front().first.x, result.front().first.y);
        glVertex2d(result.front().second.x, result.front().second.y);
        glEnd();
    }


    glBegin(GL_POINTS);
    glVertex2d(mousePosition.x, mousePosition.y);
    glEnd();

    glColor4f(1,0,0,1);

    QString mousePos;

    QTextStream(&mousePos) << "(" << mousePosition.x << "," << mousePosition.y << ")" << "__" << worldWidth << ", " << worldHeight;
    renderText(10, 20, mousePos);

    if (drawHelp) {
        renderText(10, 65, deletePoint);
        renderText(10, 80, space);
        renderText(10, 95, clear);
        renderText(10, 110, save);
        renderText(10, 125, open);
        renderText(10, 140, movement);
        renderText(10, 155, zoom);
        renderText(10, 170, sExit);
        renderText(10, 185, help);
        renderText(10, 200, res);
    }
}

void GLWidget::mousePressEvent(QMouseEvent *event) {

    keys.push_back(Nearest::give_key(mousePosition));
    points.insert(std::make_pair<double, Point>(Nearest::give_key(mousePosition), mousePosition));
    new_draw = true;
    repaint();
}
void GLWidget::mouseMoveEvent(QMouseEvent *event) {
    mousePosition.x = worldPosition.x + ((double)worldWidth/windowWidth)*event->x();
    mousePosition.y = worldPosition.y + (worldHeight - ((double)worldHeight/windowHeight)*event->y());
    repaint();
}

void GLWidget::keyPressEvent(QKeyEvent* event) {

    switch(event->key()) {
    case Qt::Key_C:
        points.clear();
        new_draw = true;
        repaint();
        break;
    case Qt::Key_D:
        if (points.size() != 0 && keys.size() != 0)
        {
            double key = keys.back();
            points.erase(key);
            keys.pop_back();

            repaint();
        }
        new_draw = true;
        break;
    case Qt::Key_S:
        QTextStream(&save) << "enter " << Nearest::save_data(points, "D:\data.txt");
        repaint();
        break;
    case Qt::Key_L:
        points.clear();
        Nearest::load_data(points, keys, "D:\data.txt");
        repaint();
        break;
    case Qt::Key_Space:
       if(points.size() > 1 && new_draw == true)
        {
            PTS data(points);
            result.clear();
            result.push_back(data.nearest());
            res.clear();
            QTextStream(&res) << "Nearest pair: (" << result.front().first.x << "," << result.front().first.y << ") <--> (" << result.front().second.x << "," << result.front().second.y << ")";

            new_draw = false;
        }
        repaint();
        break;
    case Qt::Key_Left:
        worldPosition.x -= 1;
        resizeGL(windowWidth, windowHeight);
        repaint();
        break;
    case Qt::Key_Right:
        worldPosition.x += 1;
        resizeGL(windowWidth, windowHeight);
        repaint();
        break;
    case Qt::Key_Down:
        worldPosition.y -= 1;
        resizeGL(windowWidth, windowHeight);
        repaint();
        break;
    case Qt::Key_Up:
        worldPosition.y += 1;
        resizeGL(windowWidth, windowHeight);
        repaint();
        break;
    case Qt::Key_Plus:
        worldWidth -= 1;
        resizeGL(windowWidth, windowHeight);
        repaint();
        break;
    case Qt::Key_Minus:
        worldWidth += 1;
        resizeGL(windowWidth, windowHeight);
        repaint();
        break;
    case Qt::Key_Escape:
        close();
        break;
    case Qt::Key_F1:
        drawHelp = !drawHelp;
        repaint();
        break;
    default:
        event->ignore();
        break;
    }
}
