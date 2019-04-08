//
//  placement.cpp
//  Packing
//
//  Created by Qian Hu on 2018/5/25.
//  Copyright © 2018 Naiquh Corporation. All rights reserved.
//

#include "placement.hpp"

placement::placement(cuboid & _space, block & _blk, int x, int y, int z) : space(_space), blk(_blk), cuboid(x, y, z, x + _blk.l, y + _blk.w, z + _blk.h), score(0){
    
}

void placement::verify(int L, int W, int H){
    if(x1 < 0 || x1 + blk.l > L || x2 < blk.l || x2 > L || y1 < 0 || y1 + blk.w > W || y2 < blk.w || y2 > W || z1 < 0 || z1 + blk.h > H || z2 < blk.h || z2 > H){
        std::cerr << "Invalid placement: " << *this << "\tcarton " << L << " " << W << " " << H << "\r\n";
        std::abort();
    }
}

bool operator<(const placement & a, const placement & b){
    return a.score < b.score;
}
