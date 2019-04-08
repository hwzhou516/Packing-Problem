//
//  algo_tree_search.cpp
//  Packing
//
//  Created by Qian Hu on 2018/5/25.
//  Copyright © 2018 Naiquh Corporation. All rights reserved.
//

#include "algo_tree_search.hpp"

algo_tree_search::algo_tree_search(const problem & _inst, const carton & _container, result & _res, double _timelim) : inst(_inst), container(_container), res(_res), best(_inst, _container), timelim(_timelim), start(std::clock()){
    
}

algo_tree_search::~algo_tree_search(){
    for(node * p : tree)
        delete p;
    for(node * p : inactive)
        delete p;
}

void algo_tree_search::create_root(){
    node * p = new node();
    node & root = *p;
    root.father = nullptr;
    root.arr_block = inst.arr_block;
    root.num = inst.num;
    root.container = container;
    cuboid space(0, 0, 0,container.L, container.W, container.H);
    root.arr_space.push_back(space);
    // remove blocks that are too large
    for(int i = 0; i < root.arr_block.size(); ++i){
        block & blk = root.arr_block[i];
        if(space.fit(blk.l, blk.w, blk.h) == false){
            root.arr_block[i] = root.arr_block.back();
            root.arr_block.resize(root.arr_block.size()-1);
            --i;
        }
    }
    root.update_placements(container);
    tree.push_back(p);
}

bool algo_tree_search::timeout(){
    return 1.0 * (std::clock() - start) / (CLOCKS_PER_SEC) > timelim;
}

bool algo_tree_search::solve(){
        create_root();
        node *p=tree.back();
        node &r=*p;
    tree.pop_back();
        std::sort(r.arr_placement.begin(), r.arr_placement.end());
    //int h1=(int)r.arr_placement.size()-1;
    //int h2=(int)r.arr_placement.size()-2;
    //int h3=(int)r.arr_placement.size()-3;
    //std::cout<<r.arr_placement.size()<<std::endl;
    //std::cout<<h1<<'\t'<<h2<<'\t'<<h3<<std::endl;
    int j=0;
    for(int i=(int)r.arr_placement.size()-1;i>=0&&j<=12;i--){
        placement & pack=r.arr_placement[i];
        node * pchild=new node(r,pack);
        node &child=*pchild;
        child.update_placements(container);
        
        if(child.arr_placement.size()>=12){
            tree.push_back(pchild);j=j+1;
        }
        else continue;
    }
    std::vector<node*> tree_ano;
    while(tree.size()>0){
        for(int i=1;i<=12;i++){
    node &c1hild=*tree.back();
    std::sort(c1hild.arr_placement.begin(), c1hild.arr_placement.end());
    int h11=(int)c1hild.arr_placement.size()-i;
   
    placement & pack11=c1hild.arr_placement[h11];
    node * p11child=new node(c1hild,pack11);
    node & c11hild=*p11child;
    c11hild.update_placements(container);
    //tree[1]=(p1child);
            tree_ano.push_back(p11child);
    
        }
        tree.pop_back();
    }
    
   
       /*
       int i=1,j=1;
        while(i<=3&&i<=r.arr_placement.size()){
            placement & pack1=r.arr_placement[r.arr_placement.size()-i];
            node * pchild=new node(r,pack1);
            node & child=*pchild;
            child.update_placements(container);
            tree[i]=(pchild);
            i++;
            std::sort(child.arr_placement.begin(), child.arr_placement.end());
            while (j<=3&&j<child.arr_placement.size()) {
                
                placement & subpack=child.arr_placement[child.arr_placement.size()-j];
                node * subchild=new node(child,subpack);
                node & subr= *subchild;
                subr.update_placements(container);
                tree[3*i+j]=subchild;
                j++;
            }
        }
    
    int p_val=0,p_pos=0;
    
    for(int i=0;i<tree.size();i++){
        node &a=*tree[i];
        if(a.occupy>p_val){
            p_val=a.occupy;
            p_pos=i;
        }
    }
    
    
    node *re=tree[p_pos];
    update_sol(*re);
    tree.clear();
    tree.push_back(re);
    */
    //or(int i=1;i<=9;i++){
       // tree.push_back(sample[i-1]);
    tree.clear();
    while(tree_ano.size()>0){
        node *trans=tree_ano.back();
        tree.push_back(trans);
while(tree.size() > 0 && res.feasible == false && timeout() == false){
    
    node * p = tree.back();
    node & r = *p;
    std::sort(r.arr_placement.begin(), r.arr_placement.end());
    if(r.arr_placement.size() > 0){
        const placement & pack = r.arr_placement.back();
        node * pchild = new node(r, pack);
        node & child = *pchild;
        child.update_placements(container);
        update_sol(child);
        tree.pop_back();
        inactive.push_back(p);
        tree.push_back(pchild);
        
        
        }
        else{
            tree.pop_back();
        }
    }
        tree_ano.pop_back();
    }
    
    
    return res.feasible;
}

void algo_tree_search::update_sol(const node & _a) {
    // get solution
    if (_a.occupy > best.volume) {
        best.clear();
        const node *p = &_a;
        while (p -> father != nullptr) {
            best.add_placement(p->action);
            p = p->father;
        }
        if(best.qty > inst.qty || best.volume > inst.total_volume || _a.occupy != best.volume){
            std::cerr << "error" << std::endl;
        }
        if (best.volume == inst.total_volume) {
            res.feasible = true;
            res.arr_sol.clear();
            res.arr_sol.push_back(best);
            res.compute_utl(container.volume);
        }
    }
}
