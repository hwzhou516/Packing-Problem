//
//  main.cpp
//  Packing
//
//  Created by Qian Hu on 2018/5/25.
//  Copyright © 2018 Naiquh Corporation. All rights reserved.
//

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "data.hpp"
#include "result.hpp"
#include "algo_tree_search.hpp"


int main(int argc, const char * argv[]) {
    std::cout << "Hello, World!\n";
    
    std::string fname_carton = "/Users/zhouhaowen/Desktop/文件/大四下/算法设计分析/data/Box_C.txt";
    std::string fname_product = "/Users/zhouhaowen/Desktop/文件/大四下/算法设计分析/data/Product_C.txt";
    std::string fname_order = "/Users/zhouhaowen/Desktop/文件/大四下/算法设计分析/data/Order_C.txt";
    std::string fname_benchmark = "/Users/zhouhaowen/Desktop/文件/大四下/算法设计分析/data/Benchmark.txt";
    std::string fname_result = "/Users/zhouhaowen/Desktop/文件/大四下/算法设计分析/data/result.csv";
    double timelim = 60.0;
    
    std::ofstream out;
    out.open(fname_result.c_str());
    out << "Order,qty,benchmark_carton,benchmark_utl,Your_carton,Your_utl,time" << std::endl;
    out.close();
    
    data d;
    d.read_data(fname_carton, fname_product, fname_order, fname_benchmark);
    d.prepare();
    
    for(int qty = 5; qty <= 20; ++qty){
        for(auto & p : d.map_inst[qty]){
//            if(p.first!="620000044104820000") continue;
            problem & inst = p.second;
            result r(inst);
             d.generate_blocks(inst);
            int fes=1;
            for(const carton & c : d.arr_carton){
                if(d.fit(inst, c) == false) continue;
                fes=0;
                algo_tree_search algo(inst, c, r, timelim);
                if(algo.solve()) break;
            }
            if(r.feasible){
                // output
                std::ofstream out;
                out.open(fname_result.c_str(), std::ofstream::out | std::ofstream::app);
                out << r << std::endl;
                out.close();
            }
    
            //if(fes==0 && !r.feasible)
               std::cout << r << std::endl;
            
            
            if(fes==1){
                
                int t=0,i=1;
                std::map<int,int> new_map;
                std::map<int,int> left_map;
                
                for(auto const &e : inst.num){
                    for(i=1;i<=e.second;i++){
                    t=t+d.map_product[e.first].volume*i;
                        if(t>d.arr_carton.back().volume) break;
                  }
                    new_map[e.first]=i-1;
                }
                
                for(auto const &e: inst.num){
                    left_map[e.first]=e.second-new_map[e.first];
                }
                
                problem inst1, inst2;
                inst1=p.second;inst2=p.second;
                int qty=0,total_volume=0,actual_volume=0;
                
                for(auto const & e:new_map){
                    qty=qty+e.second;
                    total_volume=total_volume+e.second*d.map_product[e.first].volume;
                    actual_volume=actual_volume+e.second*d.map_product[e.first].actual_volume;
                }
                
                inst1.qty=qty;inst1.total_volume=total_volume;inst1.actual_total_volume=actual_volume;
                
                qty=0;total_volume=0;actual_volume=0;
                
                for(auto const & e:left_map){
                    qty=qty+e.second;
                    total_volume=total_volume+e.second*d.map_product[e.first].volume;
                    actual_volume=actual_volume+e.second*d.map_product[e.first].actual_volume;
                }
                
                inst2.qty=qty;inst2.total_volume=total_volume;inst2.actual_total_volume=actual_volume;
                
                result r1(inst1),r2(inst2);
                d.generate_blocks(inst1);
                d.generate_blocks(inst2);
                algo_tree_search algo1(inst1,d.arr_carton.back(),r1,timelim);
                algo1.solve();
                
                if(r1.feasible){
                    // output
                    std::ofstream out;
                    out.open(fname_result.c_str(), std::ofstream::out | std::ofstream::app);
                    out << r1 << std::endl;
                    out.close();
                }
                std::cout << r1 << std::endl;
                for(const carton & c : d.arr_carton){
                    if(d.fit(inst2, c) == false) continue;
                    algo_tree_search algo2(inst2, c, r2, timelim);
                    if(algo2.solve()) break;
                }
                if(r2.feasible){
                    // output
                    std::ofstream out;
                    out.open(fname_result.c_str(), std::ofstream::out | std::ofstream::app);
                    out << r2 << std::endl;
                    out.close();
                }
                std::cout<<'r1  r2'<<std::endl;
                
           }
        
      }
    }
    return 0;
}
