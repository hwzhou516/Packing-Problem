//
//  result.cpp
//  Packing
//
//  Created by Qian Hu on 2018/5/25.
//  Copyright © 2018 Naiquh Corporation. All rights reserved.
//

#include "result.hpp"

solution::solution(const problem & _inst, const carton & _c) : inst(_inst), container(_c), volume(0), qty(0), actual_volume(0){
    
}

void solution::clear(){
    arr_placement.clear();
    qty = volume = 0;
    actual_volume = utl = 0;
}

void solution::add_placement(const placement &pack){
    arr_placement.push_back(pack);
    qty += pack.blk.qty;
    volume += pack.blk.volume;
    actual_volume += pack.blk.actual_volume;
    utl = 100.0 * actual_volume / container.volume;
}

void solution::verify(){
    for(int i = 0; i < arr_placement.size(); ++i){
        for(int j = i + 1; j < arr_placement.size(); ++j){
            if(arr_placement[i].intersects(arr_placement[j])){
                std::cerr << "Invalid solution: overlap found " << arr_placement[i] << "\t" << arr_placement[j] << "\r\n";
                std::abort();
            }
        }
        arr_placement[i].verify(container.L, container.W, container.H);
    }
}

result::result(const problem & _inst) : inst(_inst), utl(0), feasible(false), start(std::clock()){
    
}

void result::compute_utl(double actual_volume){
    double vol = 0;
    for(solution & sol : arr_sol)
        vol += sol.actual_volume;
    utl = 100.0 * vol / actual_volume;
    time = 1.0 * (std::clock() - start) / CLOCKS_PER_SEC;
}

std::ostream & operator<<(std::ostream & out, const result & r){
    out << r.inst.oid << "," << r.inst.qty << "," << r.inst.benchmark_sol_carton << "," << r.inst.benchmark_sol_utl;
    if(r.arr_sol.size() < 1){
        out << ",-,0.0";
    }else{
        out << "," << r.arr_sol[0].container.cid;
        for(int i = 1; i < r.arr_sol.size(); ++i){
            out << "_" << r.arr_sol[1].container.cid;
        }
        out << "," << r.utl;
    }
    out << "," << r.time;
    return out;
}
