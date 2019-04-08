//
//  data.hpp
//  Packing
//
//  Created by Qian Hu on 2018/5/25.
//  Copyright © 2018 Naiquh Corporation. All rights reserved.
//

#ifndef data_hpp
#define data_hpp

#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <iostream>
#include "block.hpp"
#include "problem.hpp"

class carton{
public:
    carton(){};
    ~carton(){};
    
public:
    std::string cid; // carton id
    int L;
    int W;
    int H;
    int volume;
};

class product{
public:
    product(){};
    product(int _pid, double _l, double _w, double _h);
    ~product(){};
    
public:
    int pno; // product no
    double actual_volume;
    int l;
    int w;
    int h;
    int volume;
    std::vector<rotated_box> arr_rbox;
    
private:
    double actual_l;
    double actual_w;
    double actual_h;
};

class data{
public:
    data(){};
    ~data(){};
    void read_data(const std::string & fname_carton, const std::string & fname_product, const std::string & fname_order, const std::string & fname_benchmark);
    void prepare();
    bool fit(const problem & inst, const carton & c);
    void generate_blocks(problem & inst);
    
public:
    std::vector<carton> arr_carton;
    std::map<int, product> map_product;
    std::map<int, std::map<std::string, problem> > map_inst;
};

bool operator<(const carton & a, const carton & b);
std::ostream & operator<<(std::ostream & out, const carton & c);

#endif /* data_hpp */
