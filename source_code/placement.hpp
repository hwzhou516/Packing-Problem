//
//  placement.hpp
//  Packing
//
//  Created by Qian Hu on 2018/5/25.
//  Copyright © 2018 Naiquh Corporation. All rights reserved.
//

#ifndef placement_hpp
#define placement_hpp

#include "block.hpp"
#include "cuboid.hpp"

class placement : public cuboid{
public:
    placement(){};
    placement(cuboid & _space, block & _blk, int x, int y, int z);
    ~placement(){};
    void verify(int L, int W, int H);
    
public:
    block blk;
    cuboid space;
    int score;
};


bool operator< (const placement & a, const placement & b);

#endif /* placement_hpp */
