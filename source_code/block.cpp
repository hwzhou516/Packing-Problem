//
//  block.cpp
//  Packing
//
//  Created by Qian Hu on 2018/5/25.
//  Copyright © 2018 Naiquh Corporation. All rights reserved.
//

#include "block.hpp"

rotated_box::rotated_box(int _l, int _w, int _h, int _orientation) : orientation(_orientation){
    switch (orientation) {
        case 1: l = _l; w = _w; h = _h; break;
        case 2: l = _w; w = _l; h = _h; break;
        case 3: l = _l; w = _h; h = _w; break;
        case 4: l = _h; w = _l; h = _w; break;
        case 5: l = _w; w = _h; h = _l; break;
        case 6: l = _h; w = _w; h = _l; break;
    }
}


block::block(const rotated_box & box) : l(box.l), w(box.w), h(box.h), volume(box.volume), qty(box.qty), actual_volume(box.actual_volume){
    num[box.pno] = 1;
}

bool block::active(std::map<int, int> & _num){
    for(const auto & p : num){
        if(p.second > _num[p.first]){
            return false;
        }
    }
    return true;
}

bool operator==(const block & a, const block & b){
    return a.num == b.num && a.l == b.l && a.w == b.w && a.h == b.h;
}
