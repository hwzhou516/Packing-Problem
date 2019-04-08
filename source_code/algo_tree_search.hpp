//
//  algo_tree_search.hpp
//  Packing
//
//  Created by Qian Hu on 2018/5/25.
//  Copyright © 2018 Naiquh Corporation. All rights reserved.
//

#ifndef algo_tree_search_hpp
#define algo_tree_search_hpp

#include "problem.hpp"
#include "data.hpp"
#include "result.hpp"
#include "node.hpp"
#include <ctime>

class algo_tree_search{
public:
    algo_tree_search(const problem & _inst, const carton & _container, result & _res, double _timelim);
    ~algo_tree_search();
    void create_root();
    bool timeout();
    bool solve();
    void update_sol(const node & _a);
    
public:
    result & res;
    solution best;
    std::vector<node *> tree;
    std::vector<node *> inactive;
    std::clock_t start;
    
private:
    const problem & inst;
    const carton & container;
    double timelim;
};

#endif /* algo_tree_search_hpp */
