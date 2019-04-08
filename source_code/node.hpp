//
//  node.hpp
//  Packing
//
//  Created by Qian Hu on 2018/5/25.
//  Copyright © 2018 Naiquh Corporation. All rights reserved.
//

#ifndef node_hpp
#define node_hpp

#include <vector>
#include <map>
#include "data.hpp"
#include "cuboid.hpp"
#include "block.hpp"
#include "placement.hpp"

class node{
public:
    node();
    node(const node & parent, const placement & pack);
    ~node(){};
    void update_placements(const carton & container);
    void compute_score(placement & pack, const carton & container);
    
public:
    std::vector<cuboid> arr_space;
    std::vector<block> arr_block;
    std::vector<placement> arr_placement;
    std::map<int, int> num;
    carton container;
    placement action;
    int occupy;
    const node * father;
    int no;
    
private:
    static int node_cnt;
};

#endif /* node_hpp */
