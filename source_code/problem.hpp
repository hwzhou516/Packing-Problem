//
//  problem.hpp
//  Packing
//
//  Created by Qian Hu on 2018/5/25.
//  Copyright © 2018 Naiquh Corporation. All rights reserved.
//

#ifndef problem_hpp
#define problem_hpp

#include <vector>
#include <string>
#include <map>
#include "block.hpp"

class problem{
public:
    problem();
    ~problem(){};
    
public:
    std::string oid; // order id
    int qty;
    int total_volume;
    double actual_total_volume;
    std::map<int, int> num;
    std::string benchmark_sol_carton;
    double benchmark_sol_utl;
    
    std::vector<block> arr_block;
};

#endif /* problem_hpp */
