#include "closerpair.h"
#include "utilities.h"

ClosetPair minDistancePairBruteForce(std::vector<Point> &arr)
{
    ClosetPair closetP(arr[0], arr[1]);
    double dValue= 0;

    for(int i = 0; i < arr.size() - 1; i++)
    {
        for(int j = i + 1; j < arr.size(); j++)
        {
            dValue = std::sqrt((arr[j].x - arr[i].x)*(arr[j].x - arr[i].x) + (arr[j].y - arr[i].y)*(arr[j].y - arr[i].y));
            if(dValue < closetP.d)
            {
                closetP.d = dValue;
                closetP.p1 = arr[i];
                closetP.p2 = arr[j];

            }
        }
    }

    return closetP;
}

bool compareX(Point p1, Point p2)
{
    return p1.x < p2.x;
}

bool compareY(Point p1, Point p2)
{
    return p1.y < p2.y;
}

std::vector<ClosetPair> closestSplitPair(std::vector<Point> &px, std::vector<Point> &py, double delta)
{
     std::vector<ClosetPair> result;
     double min = delta;
     int size = px.size();

     std::vector<Point> Sy;
     int midPointX = px[size/2].x;

     for(int i = 0; i < size; i++)
     {
         if(std::abs(py[i].x - midPointX) < delta)
         {
             Sy.push_back(py[i]);

             //std::cout<<"Sy:(x,y) = ("<<py[i].x<<","<<py[i].y<<")"<<std::endl;
         }
     }
     std::cout<<"Sy size = "<<Sy.size()<<std::endl;
     if(Sy.size() > 2)
     {
         ClosetPair closetPair(Sy[0], Sy[1]);
         for(int i = 0; i < Sy.size()-1; i++)
         {
             for(int j = i + 1; j < Sy.size(); j++)
             {
                 ClosetPair value(Sy[i], Sy[j]);
                 if(value.d < closetPair.d)
                 {
                     closetPair = value;
                 }
             }
         }
         result.push_back(closetPair);

//         for(int i = 0; i < Sy.size() - 7; i++)
//         {
//             for(int j = ; j < 7; j++)
//             {
//                  ClosetPair pairItem(Sy[i],Sy[j]);

//                 if(pairItem.d < min)
//                 {
//                     pair = pairItem;
//                     min = pairItem.d ;
//                 }
//             }
//         }
     }
      return result;
}

ClosetPair minClosesPair(std::vector<Point> &px, std::vector<Point> &py)
{
    ClosetPair result;
    if(px.size() == 2)
    {
        result = minDistancePairBruteForce(px);
        return result;
    }

    std::vector<Point> Qx(px.begin(), px.begin() + px.size()/2);
    std::vector<Point> Qy(py.begin(), py.begin() + px.size()/2);

    std::vector<Point> Rx(px.begin() + px.size()/2, px.end());
    std::vector<Point> Ry(py.begin() + py.size()/2, py.end());
    ClosetPair pair1;
    pair1 = minClosesPair(Qx, Qy);

    ClosetPair pair2;
    pair2 = minClosesPair(Rx, Ry);

    double delta =( pair1.d > pair2.d)? pair1.d: pair2.d;

    std::vector<ClosetPair> pair3;
    pair3 = closestSplitPair(px, py, delta);

    result = (pair1.d < pair2.d)? pair1: pair2;

    if(!pair3.empty())
    {
        result = result.d < pair3[0].d ? result : pair3[0];
    }


    return result;
}

ClosetPair minDistancePair2(std::vector<Point> &arr)
{
     ClosetPair closetP;
    std::vector<Point> px(arr.begin(), arr.end());
    std::vector<Point> py(arr.begin(), arr.end());

    std::cout<<"MiddleX:"<<px[px.size()/2].x<<std::endl;

    std::sort(px.begin(), px.end(),compareX);
    std::sort(py.begin(), py.end(),compareY);
    closetP = minClosesPair(px,py);

     return closetP;
}

void testClosestPair()
{
    //vector<Point> P = {{2, 3}, {12, 30}, {40, 50}, {5, 1}, {12, 10}, {3, 4}};
    int size = 256;
    vector<Point> P(size);
    srand( (unsigned)time(NULL) );

    for(int i = 0; i < size; i++)
    {
        P[i].x = rand()%1000;
        P[i].y = rand()%1000;
    }

    writeToFile(P);
    ClosetPair expectedCloser, autualCloser;
    expectedCloser = minDistancePairBruteForce(P);


    autualCloser = minDistancePair2(P);

    cout<<"====================minDistancePairBruteForce==================="<<endl;
    cout<<"Poin1: "<<expectedCloser.p1.x<<","<<expectedCloser.p1.y<<endl;
    cout<<"Poin2: "<<expectedCloser.p2.x<<","<<expectedCloser.p2.y<<endl;

    cout<<"Min distance pair: "<<expectedCloser.d<<endl;
    cout<<"================================================================="<<endl;

     cout<<"====================minDistancePair2==================="<<endl;
    cout<<"Poin1: "<<autualCloser.p1.x<<","<<autualCloser.p1.y<<endl;
    cout<<"Poin2: "<<autualCloser.p2.x<<","<<autualCloser.p2.y<<endl;

    cout<<"Min distance pair: "<<autualCloser.d<<endl;
    cout<<"================================================================="<<endl;
}
