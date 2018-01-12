#ifndef __POINT_H
#define __POINT_H

#include <math.h>

class Point{
    public:
        int x,y;
        Point();
        Point(int,int);
        double dist_to(Point&);
        bool operator <(const Point& rhs) const;
};


// Point.cpp (cmake & visual studios not linking cpp files correctly)
Point::Point(){
    x = 0;
    y = 0;
}

Point::Point(int _x, int _y){
    x = _x;
    y = _y;
}

double Point::dist_to(Point& other){
  return sqrt((x-other.x)^2 + (y-other.y)^2);
}



bool Point::operator <(const Point& rhs) const{
    return (x < rhs.x) || ((x == rhs.x) && (y < rhs.y));
}

#endif

