//
//  block.hpp
//  Packing
//
//  Created by Qian Hu on 2018/5/25.
//  Copyright © 2018 Naiquh Corporation. All rights reserved.
//

#ifndef block_hpp
#define block_hpp

#include <stdio.h>
#include <map>

class rotated_box{
public:
    rotated_box(){};
    rotated_box(int _l, int _w, int _h, int _orientation);
    ~rotated_box(){};
    
public:
    int pno;
    int orientation;
    int l, w, h;
    int qty, volume;
    double actual_volume;
};

class block{
public:
    block(){};
    block(const rotated_box & box);
    ~block(){};
    bool active(std::map<int, int> & _num);
    
public:
    int l, w, h;
    int qty, volume;
    double actual_volume;
    std::map<int, int> num;
};

bool operator==(const block & a, const block & b);

#endif /* block_hpp */
