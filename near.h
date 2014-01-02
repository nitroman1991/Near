#ifndef NEAR_H
#define NEAR_H


#include <vector>
#include "Point.h"

using namespace Nearest;
using Nearest::Point;
using std::vector;
using std::sort;
using std::pair;
using std::multimap;

namespace Nearest {

void print(int i);

vector<Point>::iterator find_place(Point const &p, vector<Point> &target);

void copy_multimap(std::map<double, Point>::iterator first, std::map<double, Point>::iterator last, vector<Point> &y);

void copy_vector(std::map<double, Point>::iterator first, std::map<double, Point>::iterator last, vector<Point> &y);

inline bool is_equal(pair<Point, Point> pr);

bool sort_by_x(Point a, Point b);

bool sort_by_y(Point a, Point b);

int save_data(std::map<double, Point> const &data, const char* filename);

void load_data(std::map<double, Point> &data, std::vector<double> &keys, char const *filename);

class PTS{
public:
    PTS();

    PTS(std::map<double, Point> &data);

    PTS(pair<vector<Point>::iterator, vector<Point>::iterator> x, vector<Point> &y);

    void make_left_task(pair<vector<Point>::iterator, vector<Point>::iterator> &x, Point &median, vector<Point> &y);

    void make_right_task(pair<vector<Point>::iterator, vector<Point>::iterator> &x, Point &median, vector<Point> &y);

    ~PTS();

    int median_index() const;

    Point &get_median();

    pair<vector<Point>::iterator, vector<Point>::iterator> left_half_iterators();

    pair<vector<Point>::iterator, vector<Point>::iterator> right_half_iterators();

    vector<Point> &get_y();

    pair<Point, Point> nearest ();


private:

	vector<Point> by_x;
	vector<Point> by_y;
};

}

#endif
