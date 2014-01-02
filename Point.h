#ifndef POINT_H
#define POINT_H

#include<math.h>

namespace Nearest {

struct Point {
    int x;
    int y;

    Point() {}

    Point(int x, int y) {
        this -> x = x;
        this -> y = y;
    }
    bool operator< (Point &b){
        return (x < b.x || (x == b.x && y < b.y));
    }

    Point operator + (Point& p) {
        Point r;
        r.x = x + p.x;
        r.y = y + p.y;
        return r;
    }

    Point operator - (Point& p) {
        Point r;
        r.x = x - p.x;
        r.y = y - p.y;
        return r;
    }

	Point operator -= (Point& p) {
        x -= p.x;
        y -= p.y;
        return *this;
    }

    Point operator * (int i) {
        Point r;
        r.x = x*i;
        r.y = y*i;
        return r;
    }

    Point operator / (int i) {
        Point r;
        r.x = x/i;
        r.y = y/i;
        return r;
    }

	double dist() const
	{
		return sqrt(x*x + y*y);
	}

    bool operator == (Point& p) {
        if (x == p.x && y == p.y)
            return true;
        else
            return false;
    }

    bool operator != (Point& p) {
        if(x == p.x && y == p.y)
            return false;
        else
            return true;
    }
};

double give_key(Point const &a);

}

#endif
