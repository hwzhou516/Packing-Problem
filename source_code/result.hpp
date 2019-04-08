//
//  result.hpp
//  Packing
//
//  Created by Qian Hu on 2018/5/25.
//  Copyright © 2018 Naiquh Corporation. All rights reserved.
//

#ifndef result_hpp
#define result_hpp

#include <string>
#include <vector>
#include <ostream>
#include <ctime>
#include "problem.hpp"
#include "placement.hpp"
#include "data.hpp"

class solution{
public:
    solution(const problem & _inst, const carton & _c);
    ~solution(){};
    void clear();
    void add_placement(const placement & pack);
    void verify();
    
public:
    const problem & inst;
    const carton & container;
    std::vector<placement> arr_placement;
    int qty;
    int volume;
    double actual_volume;
    double utl;
};

class result{
public:
    result(const problem & _inst);
    ~result(){};
    void compute_utl(double actual_volume);
    
public:
    const problem & inst;
    std::vector<solution> arr_sol;
    int performance; // 0 worse; 1 tie; 2 better
    double time;
    double utl; // space utlization
    bool feasible;
private:
    std::clock_t start;
};

std::ostream & operator<<(std::ostream & out, const result & r);

#endif /* result_hpp */
