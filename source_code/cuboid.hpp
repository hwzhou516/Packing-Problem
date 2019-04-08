//
//  cuboid.hpp
//  Packing
//
//  Created by Qian Hu on 2018/5/25.
//  Copyright © 2018 Naiquh Corporation. All rights reserved.
//

#ifndef cuboid_hpp
#define cuboid_hpp

#include <iostream>

class cuboid{
    
public:
    cuboid(){};
    cuboid(int _x1, int _y1, int _z1, int _x2, int _y2, int _z2);
    ~cuboid(){};
    bool fit(int l, int w, int h);
    bool intersects(const cuboid & s) const;
    int compute_shared_area(const cuboid & other) const;
    
public:
    int x1;
    int y1;
    int z1;
    int x2;
    int y2;
    int z2;
};

bool operator==(const cuboid & a, const cuboid & b);
std::ostream & operator<<(std::ostream & out, const cuboid & c);

#endif /* cuboid_hpp */
