//
//  node.cpp
//  Packing
//
//  Created by Qian Hu on 2018/5/25.
//  Copyright © 2018 Naiquh Corporation. All rights reserved.
//

#include "node.hpp"

int node::node_cnt = 0;

node::node() : father(nullptr), no(node_cnt++), occupy(0){
}

node::node(const node & parent, const placement & pack) : father(&parent), no(node_cnt++), action(pack), occupy(parent.occupy + pack.blk.volume), arr_block(parent.arr_block), arr_placement(parent.arr_placement), num(parent.num){
    
    // update num
    for(auto & e : pack.blk.num){
        if(num[e.first] < e.second){
            std::cerr << "blk invalid\r\n";
        }
        num[e.first] -= e.second;
        if(num[e.first] == 0){
            num.erase(e.first);
        }
    }
    
    // update available blocks
    std::vector<block> infeas_block;
    for(int i = 0; i < arr_block.size(); ++i){
        if(arr_block[i].active(num) == false){
            infeas_block.push_back(arr_block[i]);
            arr_block[i] = arr_block.back();
            arr_block.pop_back(); // arr_block.resize(arr_block.size() - 1);
            --i;
        }
    }
    
    // update available placements
    std::vector<cuboid> occupied_space;
    occupied_space.push_back(pack.space);
    for(int i = 0; i < arr_placement.size(); ++i){
        placement & p = arr_placement[i];
        bool feasible = true;
        if(pack.intersects(p.space)){
            if(std::find(occupied_space.begin(), occupied_space.end(), p.space) == occupied_space.end()){
                occupied_space.push_back(p.space);
            }
            feasible = false;
        }
        //        if(feasible == false || std::find(infeas_block.begin(), infeas_block.end(), p.blk) != infeas_block.end()){
        if(feasible == false || arr_placement[i].blk.active(num) == false){
            arr_placement[i] = arr_placement.back();
            arr_placement.pop_back(); // arr_placement.resize(arr_placement.size() - 1);
            --i;
        }
    }
    
    // update spaces
    for(const cuboid & c : occupied_space){
        if(pack.x1 > c.x1){
            cuboid space(c.x1, c.y1, c.z1, pack.x1, c.y2, c.z2);
            arr_space.push_back(space);
        }
        if(pack.y1 > c.y1){
            cuboid space(c.x1, c.y1, c.z1, c.x2, pack.y1, c.z2);
            arr_space.push_back(space);
        }
        if(pack.z1 > c.z1){
            cuboid space(c.x1, c.y1, c.z1, c.x2, c.y2, pack.z1);
            arr_space.push_back(space);
        }
        if(pack.x2 < c.x2){
            cuboid space(pack.x2, c.y1, c.z1, c.x2, c.y2, c.z2);
            arr_space.push_back(space);
        }
        if(pack.y2 < c.y2){
            cuboid space(c.x1, pack.y2, c.z1, c.x2, c.y2, c.z2);
            arr_space.push_back(space);
        }
        if(pack.z2 < c.z2){
            cuboid space(c.x1, c.y1, pack.z2, c.x2, c.y2, c.z2);
            arr_space.push_back(space);
        }
    }
}

void node::update_placements(const carton & container){
    std::vector<bool> s(arr_space.size(), false);
    std::vector<bool> b(arr_block.size(), false);
    for (int h = 0; h < arr_space.size(); ++h) {
        cuboid & space = arr_space[h];
        for (int i = 0; i < arr_block.size(); ++i) {
            block & box = arr_block[i];
            if (space.fit(box.l, box.w, box.h)) {
                b[i] = true;
                s[h] = true;
                
                // start to pack
                std::vector<int> xlist;
                xlist.push_back(space.x1);
                if (space.x1 + box.l < space.x2) {
                    xlist.push_back(space.x2 - box.l);
                }
                std::vector<int> ylist;
                ylist.push_back(space.y1);
                if (space.y1 + box.w < space.y2) {
                    ylist.push_back(space.y2 - box.w);
                }
                std::vector<int> zlist;
                zlist.push_back(space.z1);
                if (space.z1 + box.h < space.z2) {
                    zlist.push_back(space.z2 - box.h);
                }
                // put the box in a corner inside the cuboid space, there are fewer
                // than 8 corners in the space
                for (int x : xlist) {
                    for (int y : ylist) {
                        for (int z : zlist) {
                            placement pack(space, box, x, y, z);
                            compute_score(pack, container);
                            arr_placement.push_back(pack);
                        }
                    }
                }
            }
        }
        // sort by score
        std::sort(arr_placement.begin(), arr_placement.end());
    }
    
    int h = (int) s.size() - 1;
    while(h >= 0){
        if(s[h] == false){
            arr_space.erase(arr_space.begin() + h);
        }
        --h;
    }
    
    int i = (int) b.size() - 1;
    while( i >= 0){
        if(b[i] == false){
            arr_block.erase(arr_block.begin() + i);
        }
        --i;
    }
}

void node::compute_score(placement &pack, const carton &container){
    pack.score = 0;
    if (pack.x1 == 0)
        pack.score += pack.blk.w * pack.blk.h;
    if (pack.y1 == 0)
        pack.score += pack.blk.l * pack.blk.h;
    if (pack.z1 == 0)
        pack.score += pack.blk.l * pack.blk.w;
    if (pack.x2 == container.L)
        pack.score += pack.blk.w * pack.blk.h / 2;
    if (pack.y2 == container.W)
        pack.score += pack.blk.l * pack.blk.h / 2;
    if (pack.z2 == container.H)
        pack.score += pack.blk.l * pack.blk.w / 2;
    
    const node *p = this;
    while(p->father != nullptr){
        pack.score += pack.compute_shared_area(p->action);
        p = p->father;
    }
}
