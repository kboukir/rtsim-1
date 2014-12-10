#ifndef __GRAPH2D_H__
#define __GRAPH2D_H__
#include <list>
#include <string>


class Graph2d
{
    public:
        struct Point {
            unsigned int x;
            unsigned int y;
        };
        Graph2d(std::list<Point> &points, std::string x_name, std::string y_name);
        void writeInSvgFile(std::string filename);
        void writeInGnuplotFile(std::string filename);
        
    private:
        std::list<Point> _points;
        std::string _x_name;
        std::string _y_name;
        
        void sortPointsByXValue();
        unsigned int maxValueOfY();
};

#endif
