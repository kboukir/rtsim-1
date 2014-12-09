#include "graph2d.h"
#include <string>
#include <iostream>

int main(int argc, char **argv) {
    std::list<Graph2d::Point> points;
    Graph2d::Point pt1, pt2, pt3;
    pt1.x = 2;
    pt1.y = 10;
    pt2.x = 1;
    pt2.y = 5;
    pt3.x = 3;
    pt3.y = 15;
    points.push_back(pt1);
    points.push_back(pt2);
    points.push_back(pt3);
    std::string a("# of tasks");
    std::string b("switching time (%)");
    Graph2d graph(points, a, b);
    graph.writeInFile("out.svg");

    return 0;
}