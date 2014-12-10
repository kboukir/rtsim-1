#include "graph2d.h"
#include <iostream>
#include <fstream>

Graph2d::Graph2d(std::list< Graph2d::Point >& points, std::string x_name, std::string y_name) 
: _points(points), _x_name(x_name), _y_name(y_name)
{
    sortPointsByXValue();
}

void Graph2d::writeInSvgFile(std::string filename)
{
    std::ofstream svg(filename.c_str());
    
    std::list<Point>::iterator it = _points.end();
    --it;
    
    const unsigned int x_max = it->x;
    const unsigned int y_max = maxValueOfY();
    const unsigned int gap = 15;
    const unsigned int x_label_size = 15;
    const unsigned int y_label_size = _y_name.size()*5;
    const unsigned int line_width = 1;
    const unsigned int number_width = 5;
    const unsigned int number_height = 7;
    const unsigned int tick_half_part = 2;
    const unsigned int point_radius = 1;
    
    const unsigned int width = y_label_size + number_height + tick_half_part + line_width + (x_max+1)*gap + (x_max+1)*line_width;
    const unsigned int height = x_label_size + number_height + tick_half_part + line_width + (y_max+1)*gap + (y_max+1)*line_width;
    
    
    // SVG header
    svg << "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n";
    svg << "<svg width=\"" << width << "\" "
        << "height=\"" << height << "\" "
        << "version=\"1.1\" xmlns:svg=\"http://www.w3.org/2000/svg\" xmlns=\"http://www.w3.org/2000/svg\">\n";
        
    
    unsigned int x1, y1, x2, y2;
    // Draw x axis
    x1 = y_label_size+number_width+tick_half_part;
    y1 = height-(x_label_size+number_height+tick_half_part)-1;
    x2 = width-1;
    y2 = y1;
    svg << "<line x1=\"" << x1 << "\" y1=\"" << y1 << "\" x2=\"" << x2 << "\" y2=\"" << y2 << "\" stroke=\"black\" />\n";
    // Draw y axis
    x1 = y_label_size+number_width+tick_half_part;
    y1 = height-(x_label_size+number_height+tick_half_part)-1;
    x2 = x1;
    y2 = 0;
    svg << "<line x1=\"" << x1 << "\" y1=\"" << y1 << "\" x2=\"" << x2 << "\" y2=\"" << y2 << "\" stroke=\"black\" />\n";
    
    // Draw ticks on x axis
    for(unsigned int i=1; i<=x_max; ++i)
    {
        x1 = y_label_size+number_width+tick_half_part+gap*i+i;
        y1 = height-(x_label_size+number_height+tick_half_part)-1-tick_half_part;
        x2 = x1;
        y2 = height-(x_label_size+number_height+tick_half_part)-1+tick_half_part;
        svg << "<line x1=\"" << x1 << "\" y1=\"" << y1 << "\" x2=\"" << x2 << "\" y2=\"" << y2 << "\" stroke=\"black\" />\n";
        // Draw number
        svg << "<text x=\"" << x1-number_width << "\" y=\"" << y2+number_height << "\">";
        svg << i;
        svg << "</text>\n";
    }
    
    // Draw ticks on y axis
    for(unsigned int i=1; i<=y_max; ++i)
    {
        x1 = y_label_size+number_width+tick_half_part-tick_half_part;
        y1 = height-(x_label_size+number_height+tick_half_part+gap*i+i)-1;
        x2 = y_label_size+number_width+tick_half_part+tick_half_part;
        y2 = y1;
        svg << "<line x1=\"" << x1 << "\" y1=\"" << y1 << "\" x2=\"" << x2 << "\" y2=\"" << y2 << "\" stroke=\"black\" />\n";
        // Draw number
        svg << "<text x=\"" << x1-(number_width*std::to_string(i).size()) << "\" y=\"" << (unsigned int) y2+number_height/2 << "\">";
        svg << i;
        svg << "</text>\n";
    }
    
    // Draw labels
    x1 = 0;
    y1 = height/2;
    svg << "<text x=\"" << x1 << "\" y=\"" << y1 << "\">";
    svg << _y_name;
    svg << "</text>\n";
    
    x1 = width/2;
    y1 = height-(x_label_size/2)-1;
    svg << "<text x=\"" << x1 << "\" y=\"" << y1 << "\">";
    svg << _x_name;
    svg << "</text>\n";
    
    // Draw points
    for(it=_points.begin(); it!=_points.end(); ++it)
    {
        x1 = y_label_size+number_width+tick_half_part+gap*it->x+it->x;
        y1 = height-(x_label_size+number_height+tick_half_part+gap*it->y+it->y)-1;
        svg << "<circle cx=\"" << x1 << "\" cy=\"" << y1 << "\" r=\"" << point_radius << "\" stroke=\"black\" fill=\"black\" />";
    }
    
    svg << "</svg>\n";
    
    svg.flush();
    svg.close();
}


void Graph2d::writeInGnuplotFile(std::string filename) {
    std::ofstream gnuplot_script(filename.c_str());
    std::string gnuplot_data_filename(filename + ".dat");
    std::ofstream gnuplot_data(gnuplot_data_filename.c_str());
    
    gnuplot_data << "#Data for " << filename.c_str() << "\n";
    for(std::list<Point>::iterator it=_points.begin(); it!=_points.end(); ++it) {
        gnuplot_data << it->x << " " << it->y << "\n";
    }
    
    gnuplot_script << "set autoscale\n"
        << "set terminal pdf\n"
        << "set output \"" << filename + ".pdf" << "\"\n"
        << "unset log\n"
        << "unset label\n"
        << "set xtic auto\n"
        << "set ytic auto\n"
        << "set xlabel \"" << _x_name << "\"\n"
        << "set ylabel \"" << _y_name << "\"\n"
        << "plot \"" << gnuplot_data_filename << "\" notitle with linespoints\n";
        
        
    gnuplot_data.flush();
    gnuplot_data.close();
    gnuplot_script.flush();
    gnuplot_script.close();
    
}


void Graph2d::sortPointsByXValue()
{
    _points.sort([] (const Point &first, const Point &second) {
        if (first.x <= second.x)
            return true;
        else
            return false;
    });
}

unsigned int Graph2d::maxValueOfY()
{
    unsigned int max_val = _points.begin()->y;
    for(std::list<Point>::iterator it = _points.begin(); it!=_points.end(); ++it)
    {
        if(max_val < it->y)
        {
            max_val = it->y;
        }
    }
    return max_val;
}

