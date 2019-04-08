//
//  cuboid.cpp
//  Packing
//
//  Created by Qian Hu on 2018/5/25.
//  Copyright © 2018 Naiquh Corporation. All rights reserved.
//

#include "cuboid.hpp"
#include <algorithm>

cuboid::cuboid(int _x1, int _y1, int _z1, int _x2, int _y2, int _z2) : x1(_x1), y1(_y1), z1(_z1), x2(_x2), y2(_y2), z2(_z2){
    
}

bool cuboid::fit(int l, int w, int h){
    return x1 + l <= x2 && y1 + w <= y2 && z1 + h <= z2;
}

bool cuboid::intersects(const cuboid & s) const{
    if (s.x2 <= x1 || x2 <= s.x1 || s.y2 <= y1
        || y2 <= s.y1 || s.z2 <= z1 || z2 <= s.z1)
        return false;
    else
        return true;
}

int cuboid::compute_shared_area(const cuboid &other) const{
    int length = 0;
    int width = 0;
    if (x1 == other.x2 || x2 == other.x1) {
        length = std::min(y2, other.y2) - std::max(y1, other.y1);
        width = std::min(z2, other.z2) - std::max(z1, other.z1);
    } else if (y1 == other.y2 || y2 == other.y1) {
        length = std::min(x2, other.x2) - std::max(x1, other.x1);
        width = std::min(z2, other.z2) - std::max(z1, other.z1);
    } else if (z1 == other.z2 || z2 == other.z1) {
        length = std::min(x2, other.x2) - std::max(x1, other.x1);
        width = std::min(y2, other.y2) - std::max(y1, other.y1);
    }
    if (length > 0 && width > 0)
        return length * width;
    else {
        return 0;
    }
}

bool operator==(const cuboid & a, const cuboid & b){
    return a.x1 == b.x1 && a.x2 == b.x2 && a.y1 == b.y1 && a.y2 == b.y2 && a.z1 == b.z1 && a.z2 == b.z2;
}

std::ostream & operator<<(std::ostream & out, const cuboid & c){
    out << "(" << c.x1 << "," << c.y1 << "," <<c.z1 << ") ~ (" << c.x2 << "," << c.y2 << "," << c.z2 << ")";
    return out;
}
