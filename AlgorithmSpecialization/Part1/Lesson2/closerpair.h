#ifndef CLOSERPAIR_H
#define CLOSERPAIR_H
#include <vector>
#include <cmath>
#include <algorithm>

typedef struct Point
{
    int x;
    int y;
    Point():x(0), y(0){};

    Point(int xvalue, int yvalue)
    {
        x = xvalue;
        y = yvalue;
    }
}Point;

struct ClosetPair
{
    Point p1;
    Point p2;
    double d;
    ClosetPair():p1(0,0), p2(0,0), d(0.0f){};

    ClosetPair(Point pp1, Point pp2, double dValue)
    {

        p1 =pp1;
        p2 =pp2;

        d = dValue;
    }

    ClosetPair(Point pp1, Point pp2)
    {
        p1 =pp1;
        p2 =pp2;

        d = std::sqrt((pp2.x - pp1.x)*(pp2.x - pp1.x) + (pp2.y - pp1.y)*(pp2.y - pp1.y));
    }

    ClosetPair( const ClosetPair& pair)
    {
        p1 = pair.p1;
        p2 = pair.p2;
        d = pair.d;
    }


};


ClosetPair minDistancePairBruteForce(std::vector<Point>& arr);
ClosetPair minDistancePair2(std::vector<Point> &arr);

void testClosestPair();

#endif // CLOSERPAIR_H
