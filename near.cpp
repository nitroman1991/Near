#include <iostream>
#include <vector>
#include <algorithm>
#include <assert.h>
#include <fstream>
#include <map>
#include "near.h"
#include "Point.h"

#define DEBUG

using Nearest::Point;
using std::vector;
using std::sort;
using std::pair;
using std::multimap;

namespace Nearest {

void print(int i){
    std::cout << i << " ";
}

vector<Point>::iterator find_place(Point const &p, vector<Point> &target)
{
    vector<Point>::iterator i = --target.end();
    if(p.y >= (--target.end()) -> y) return target.end();	// ����� �� y ���������� ������ ���� ���������, ����� end()
    else if(p.y < (target.begin() -> y)) return target.begin();//����� �� y ���������� ������ ���� ���������, ����� begin()
    else while(i -> y > p.y) --i;
    return ++i;	//����� - ������ ������ �����, ������ ������� �� y ����������
}

void copy_multimap(std::map<double, Point>::iterator first, std::map<double, Point>::iterator last, vector<Point> &y)
{
    y.clear();
    std::map<double, Point>::iterator ptr = first;
    while(ptr != last)
    {
        y.push_back(ptr -> second);
        ++ptr;
    }	//������ std::copy, ������ ��� ��� �������� ������ ��� ����� ������, � ��� - ��� ���.
}

void copy_vector(std::vector<Point>::iterator first, std::vector<Point>::iterator last, vector<Point> &y)
{
    y.clear();
    std::vector<Point>::iterator ptr = first;
    while(ptr != last)
    {
        y.push_back(*ptr);
        ++ptr;
    }	//������ std::copy, ������ ��� ��� �������� ������ ��� ����� ������, � ��� - ��� ���.
}

inline bool is_equal(pair<Point, Point> pr)
{
    if (pr.first == pr.second) return true;
    else return false;	//��������������� - ��� �������������� ������, ����� � ��������� 1 �����(��������� - ������� ������ ���������� ���� ����� ������)
}

bool sort_by_x(Point a, Point b) {return (a.x < b.x || (a.x == b.x && a.y < b.y));}
bool sort_by_y(Point a, Point b) {return (a.y < b.y || (a.y == b.y && a.x < b.x));}

int save_data(std::map<double, Point> const &data, const char* filename)
{
    std::ofstream ofile;
    ofile.open(filename, std::ios::out);
    assert(ofile.is_open());

    ofile << data.size() << " " << std::endl;
    for(std::map<double, Point>::const_iterator i = data.begin(); i != data.end(); ++i)
        ofile << i -> second.x << " " << i -> second.y << std::endl;
    ofile.close();
    return ofile.is_open();
}

void load_data(std::map<double, Point> &data, std::vector<double> &keys, const char *filename)
{
    int num_points = 0;
    int a = 0;
    int b = 0;
    std::ifstream ifile;

    ifile.open(filename, std::ios::in);
    assert(ifile.is_open());
    assert(data.empty());

    ifile >> num_points;
    for (int i = 0; i < num_points; i++)
    {
        ifile >> a >> b;
        double key = give_key(Point(a, b));
        data.insert(std::make_pair(key, Point(a, b)));
        keys.push_back(key);
    }

}

PTS::PTS() : by_x(), by_y()
{

}

PTS::PTS(std::map<double, Point> &data)
{
        Nearest::copy_multimap(data.begin(), data.end(), by_x);
        Nearest::copy_multimap(data.begin(), data.end(), by_y);
        sort(by_x.begin(), by_x.end(), sort_by_x);
        sort(by_y.begin(), by_y.end(), sort_by_y);
}//�������� ����������� - �������� ������� ��� �������� ������ �������� ������(��������� ���� ���)

PTS::PTS(pair<vector<Point>::iterator, vector<Point>::iterator> x, vector<Point> &y)
{
    Nearest::copy_vector(x.first, x.second, by_x);
    Nearest::copy_vector(y.begin(), y.end(), by_y);
}//����������� ��� ������, ����� ��� �������� ��������������������� x � � ��� ��������.(���������� � ���� ������ �� ���������)

void PTS::make_left_task(pair<vector<Point>::iterator, vector<Point>::iterator> &x, Point &median, vector<Point> &y)
{
    by_y.clear();

    Nearest::copy_vector(x.first, x.second, by_x);
    for(vector<Point>::iterator i = y.begin(); i != y.end(); ++i)
        if(i -> x < median.x || (i -> x == median.x && i -> y < median.y)) by_y.push_back(*i);
}//������ ����� ���������

void PTS::make_right_task(pair<vector<Point>::iterator, vector<Point>::iterator> &x, Point &median, vector<Point> &y)
{
    by_y.clear();

    Nearest::copy_vector(x.first, x.second, by_x);
    for(vector<Point>::iterator i = y.begin(); i != y.end(); ++i)
        if(i -> x > median.x || (i -> x == median.x && i -> y >= median.y)) by_y.push_back(*i);
}//������ ����� ���������

PTS::~PTS()
{
    by_x.clear();
    by_y.clear();
}

int PTS::median_index() const
{
    assert(by_x.size() > 0);
    assert(by_x.size() == by_y.size());

    return (by_x.size()) / 2;
}

Point & PTS::get_median()
{
    assert(by_x.size() > 0);
    assert(by_x.size() == by_y.size());

    return by_x[by_x.size()/2];
}

pair<vector<Point>::iterator, vector<Point>::iterator> PTS::left_half_iterators()
{
    assert(by_x.size() > 0);
    assert(by_x.size() == by_y.size());

    return std::make_pair(by_x.begin(), by_x.begin() + median_index());
}

pair<vector<Point>::iterator, vector<Point>::iterator> PTS::right_half_iterators()
{
    assert(by_x.size() > 0);
    assert(by_x.size() == by_y.size());

    return std::make_pair(by_x.begin() + median_index(), by_x.end());
}

vector<Point> & PTS::get_y()
{
    return by_y;
}

pair<Point, Point> PTS::nearest ()
{
        if(by_x.size() == 1)
            return std::make_pair(by_x.front(), by_x.front());
        if(by_x.size() == 2)
            return std::make_pair(by_x.front(), by_x.back());


        PTS left_half;
        PTS right_half;

        Point med = get_median();

        pair<vector<Point>::iterator, vector<Point>::iterator> lh = left_half_iterators();
        pair<vector<Point>::iterator, vector<Point>::iterator> rh = right_half_iterators();

        left_half.make_left_task(lh, med, by_y);	//��������� ����� ��� ���� �������� �� O(n) (��� ���������� �� y ������ ���)
        right_half.make_right_task(rh, med, by_y);

        pair<Point, Point> result_left = left_half.nearest();	//������� ������� �������� 2T(n/2)
        pair<Point, Point> result_right = right_half.nearest();
        pair<Point, Point> result;

        if(is_equal(result_left)) result = result_right;
        else if(is_equal(result_right)) result = result_left;
        else result = ((result_left.first - result_left.second).dist() <= (result_right.first - result_right.second).dist() ? result_left : result_right);
        double delta = (result.first - result.second).dist();	//������� ����������� ���������� delta (��������� ������, ����� ���� �� �������� �����������)O(1)

        vector<Point> P1;
        vector<Point> P2;
        for(vector<Point>::iterator i = left_half.get_y().begin(); i != left_half.get_y().end(); ++i)
            if((med.x) - (i -> x) <= delta) P1.push_back(*i);
        for(vector<Point>::iterator i = right_half.get_y().begin(); i != right_half.get_y().end(); ++i)
            if((i -> x) - (med.x) <= delta) P2.push_back(*i);
        //��������� ������ �����-���������� ������������� ���� �� �������� �������, ��� delta O(n)

        if(P1.empty() || P2.empty())
            return result;	//���� ����� �� ������ ����, �� ���������� ������ - ������ ����� �� ������� ��������.
        else
        {
            //���� ��������� ��� ���������� ���� �� �����
            vector<Point>::iterator ptr = find_place(P1.front(), P2);	//������� �����, ������ ��������
            vector<Point>::iterator left_ptr = ptr;	//��� ��������� - ��� ������ ������ � ����� � P2
            vector<Point>::iterator right_ptr = ptr;	//�����, ��� �������� ��� ������ ����� �� P1 ������ ����� �� ����� 6 �����
            for(vector<Point>::iterator i = P1.begin(); i != P1.end(); ++i)
            {
                while(ptr != P2.end() && ptr -> y <= i -> y) ++ptr;	//��������� �����, ������ ��������(�� P2 ������������), ��� ���������� �����.
                if(ptr != P2.begin()) //��� ���� �����, ��������, ��� �������� ����� �� P2.begin() �� �����
                {
                    left_ptr = ptr - 1;
                    while((i -> y) - (left_ptr -> y) <= delta && left_ptr != P2.begin())
                    {
                        if((result.first - result.second).dist() >= (*i - *left_ptr).dist())
                            result = std::make_pair(*i, *left_ptr);
                        --left_ptr;
                    }
                    if((i -> y) - (left_ptr -> y) <= delta && (result.first - result.second).dist() >= (*i - *left_ptr).dist())
                        result = std::make_pair(*i, *left_ptr);//�� ��� ���� �� ��������� --P2.begin(), ��������� ��� ������ �������(����� *left_ptr ����� �� begin())
                }
                right_ptr = ptr;	//��� - ���� ������
                while(right_ptr != P2.end() && (right_ptr -> y) - (i -> y) <= delta)
                {
                    if((result.first - result.second).dist() >= (*right_ptr - *i).dist())
                        result = std::make_pair(*right_ptr, *i);
                    ++right_ptr;
                }

            }
        }
        return result;
}



}
