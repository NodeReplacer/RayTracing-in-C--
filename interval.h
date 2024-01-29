#ifndef INTERVAL_H
#define INTERVAL_H

class interval {
public:
    double min, max;

    interval() : min(+infinity), max(-infinity) {} // Default interval is empty
    //infinity is found in common_constants.h, which includes this file. In doing so we don't need to include
    //common_constants.h.
    
    interval(double _min, double _max) : min(_min), max(_max) {}

    bool contains(double x) const {
        return min <= x && x <= max;
    }
    
    //Does the variable 'x' fall between the min and max defined when we first created the interval.
    bool surrounds(double x) const {
        return min < x && x < max;
    }
    
    //Reduce whatever value we are looking at down to the limits of the interval.
    double clamp(double x) const {
        if (x < min) return min;
        if (x > max) return max;
        return x;
    }
    
    static const interval empty, universe;
};

const static interval empty   (+infinity, -infinity);
const static interval universe(-infinity, +infinity);

#endif