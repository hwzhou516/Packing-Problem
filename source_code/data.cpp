//
//  data.cpp
//  Packing
//
//  Created by Qian Hu on 2018/5/25.
//  Copyright © 2018 Naiquh Corporation. All rights reserved.
//

#include "data.hpp"
#include "cuboid.hpp"

product::product(int _pno, double _l, double _w, double _h) : pno(_pno), actual_l(_l), actual_w(_w), actual_h(_h), actual_volume(_l * _w * _h){
    double d = 1 - 1.0e-3;
    l = (int) (_l + d); // simple ceiling
    w = (int) (_w + d);
    h = (int) (_h + d);
    volume = l * w * h;
}

void data::prepare(){
    // sort cartons
    for(carton & c : arr_carton){
        c.volume = c.L * c.W * c.H;
    }
    std::sort(arr_carton.begin(), arr_carton.end());
//    for(carton & c : arr_carton){
//        std::cout << c << "\r\n";
//    }
    
    // rank instances by qty
    for(auto & p : map_inst){
        for(auto & q : p.second){
            // check qty
            int qty = 0;
            for(auto & r : q.second.num){
                qty += r.second;
            }
            if(qty != q.second.qty){
                std::cerr << "qty error: " << qty  << "\t" << q.second.qty << "\r\n";
            }
            // compute volume
            problem & inst  = q.second;
            inst.total_volume = 0;
            inst.actual_total_volume = 0;
            for(auto & r : q.second.num){
                product & a = map_product[r.first];
                inst.total_volume += a.volume * r.second;
                inst.actual_total_volume += a.actual_volume * r.second;
            }
        }
    }
    
    // rotate products
    for(auto & e : map_product){
        product & p = e.second;
        std::vector<bool> f(7,true);
        if(p.l == p.w){
            f[2] = f[5] = f[6] = false;
        }
        if(p.w == p.h){
            f[3] = f[4] = f[6] = false;
        }
        if(p.l == p.h){
            f[4] = f[5] = f[6] = false;
        }
        for(int i = 1; i <= 6; ++i){
            if(f[i]){
                rotated_box rbox(p.l, p.w, p.h, i);
                rbox.pno = p.pno;
                rbox.qty = 1;
                rbox.volume = p.volume;
                rbox.actual_volume = p.actual_volume;
                p.arr_rbox.push_back(rbox);
            }
        }
    }
}

bool data::fit(const problem & inst, const carton & c){
    if(inst.total_volume > c.volume) return false;
    cuboid container(0, 0, 0, c.L, c.W, c.H);
    for(const auto & e : inst.num){
        const product & p = map_product[e.first];
        bool fit = false;
        for(const rotated_box & rb : p.arr_rbox){
            if(container.fit(rb.l, rb.w, rb.h)){
                fit = true;
                break;
            }
        }
        if(fit == false)
            return false;
    }
    return true;
}

void data::generate_blocks(problem &inst){
    inst.arr_block.clear();
    // basic blocks
    for(const auto & e: inst.num){
        const product & p = map_product[e.first];
        for(const rotated_box & rb : p.arr_rbox){
            block b(rb);
            inst.arr_block.push_back(b);
        }
    }
    
    // simple blocks
    
    int L=0; int W=0; int H=0;
    for(const auto & c:arr_carton){
        if(c.L>L) L=c.L;
        if(c.W>W) W=c.W;
        if(c.H>H) H=c.H;
    }                                  /////get the maximal L,W,H
    
    std::vector<block> new_arr_block;
    for(int i=0; i<inst.arr_block.size();i++)
    {
        block b=inst.arr_block[i];
        int m=10000;
        for(auto const &e: b.num)
        {
            if(m>(inst.num[e.first]/e.second))
                m =inst.num[e.first]/e.second;
            
        }
        int X,Y,Z;
        if(m< (L/b.l)) X=m;
        else X=L/b.l;
        
        if(m<(W/b.w)) Y=m;
        else Y=W/b.w;
        
        if(m<(H/b.h)) Z=m;
        else Z=H/b.h;
        
        for(int x=1; x<=X;x++){
            for(int y=1;y<=Y;y++){
                for(int z=1;z<=Z;z++){
                    int n=x*y*z;
                    int l=x*b.l;
                    int w=y*b.w;
                    int h=z*b.h;
                    cuboid con(0,0,0,L,W,H);
                    if(n>=2 && n<=m && con.fit(l,w,h)){
                        std::map<int,int> new_map;
                        for(auto const& e: b.num){
                            new_map[e.first]=n*e.second;
                        }
                        rotated_box rbox(l,w,h,1);
                        rbox.qty = n;
                        rbox.volume = n*b.volume;
                        rbox.actual_volume = n*b.actual_volume;
                        block bb(rbox);
                        bb.num=new_map;
                        bool fes=1;
                        for(auto const &e : new_arr_block){
                            if(e==bb) fes=0;
                        }
                        if(fes==1) inst.arr_block.push_back(bb);
                    }
                }
            }
        }
    }
    //std::cout<<"error"<<std::endl;
    
    // general blocks  different kind
/*
    if(inst.qty<2) return;
    
    std::vector<block> p_block;
    std::vector<block> & b_block=inst.arr_block;
    
    double min_use=0.98;
    p_block=inst.arr_block;;
    if(inst.qty>6 && inst.arr_block.size()<10000){
        while(b_block.size()<10000){
            std::vector<block> n_block;
            for(auto & a: p_block){
                int fff=1;
                for(auto const & e : inst.num){
                if(a.num.find(e.first)!=a.num.end() ){
                    if((a.num[e.first])>e.second) fff=0;
                }
                }
                if(fff==0) continue;
                for(auto & b: b_block){
                    
                    int fbb=1;
                    for(auto const & e : inst.num){
                        if(b.num.find(e.first)!=b.num.end()){
                            if(b.num[e.first]>e.second) fbb=0;
                        }
                    }
                    if(fbb==0) continue;
                    
                    
                    
                    bool fes=1;
                    if(a==b) fes=0;
                    for(auto const & e : inst.num){
                        if(a.num.find(e.first)!=a.num.end() && b.num.find(e.first)!=b.num.end()){
                            if((a.num[e.first]+b.num[e.first])>e.second) fes=0;
                        }

                    }
                    
                    
                    if(fes==0) continue;
                    
                    int n=a.qty+b.qty;
                    
                    int l,w,h;
                    // l方向情况
                    l=a.l+b.l;
                    if(l<=L){
                        if(a.w>b.w) w=a.w; else w=b.w;
                        if(a.h>b.h) h=a.h; else h=b.h;
                        rotated_box rbox(l,w,h,1);
                        rbox.qty = n;
                        rbox.volume = a.volume+b.volume;
                        rbox.actual_volume = a.actual_volume+b.actual_volume;
                        if((rbox.volume*1.0/(l*h*w))>min_use){
                            block blo_l(rbox);
     
                            for(auto & num_a : a.num){
                                blo_l.num[num_a.first]=num_a.second;
                                
                            }
                            for(auto & num_b :b.num){
                                if(blo_l.num.find(num_b.first)!=blo_l.num.end()){
                                    blo_l.num[num_b.first]+=num_b.second;
                                }
                                else{
                                    blo_l.num[num_b.first]=num_b.second;
                                }
                                
                            }
                            int fes=1;
                            for(auto const & e: n_block){
                                if(e==blo_l) fes=0;
                            }
                            if(fes==1) n_block.push_back(blo_l);
                        }
                        
                    }
                
                    //w方向
                    w=a.w+b.w;
                    if(w<=W){
                        if(a.l>b.l) l=a.l; else l=b.l;
                        if(a.h>b.h) h=a.h; else h=b.h;
                        rotated_box rbox(l,w,h,1);
                        rbox.qty = n;
                        rbox.volume = a.volume+b.volume;
                        rbox.actual_volume = a.actual_volume+b.actual_volume;
                        if(rbox.volume*1.0/(l*h*w)>min_use){
                            block blo_w(rbox);
                            for(auto & num_a : a.num){
                                blo_w.num[num_a.first]=num_a.second;
                                
                            }
                            for(auto & num_b :b.num){
                                if(blo_w.num.find(num_b.first)==blo_w.num.end()){
                                    blo_w.num[num_b.first]=num_b.second;
                                }
                                else{
                                    blo_w.num[num_b.first]+=num_b.second;
                                }
                                
                            }
                            int fes=1;
                            for(auto const & e: n_block){
                                if(e==blo_w) fes=0;
                            }
                            if(fes==1) n_block.push_back(blo_w);
                        }
                        
                    }
                    
                    //h方向
                    h=a.h+b.h;
                    if(h<=H){
                        if(a.l>b.l) l=a.l; else l=b.l;
                        if(a.w>b.w) w=a.w; else w=b.w;
                        rotated_box rbox(l,w,h,1);
                        rbox.qty = n;
                        rbox.volume = a.volume+b.volume;
                        rbox.actual_volume = a.actual_volume+b.actual_volume;
                        if((rbox.volume*1.0/(l*h*w))>min_use){
                            block blo_h(rbox);
                            for(auto & num_a : a.num){
                                blo_h.num[num_a.first]=num_a.second;
                                
                            }
                            for(auto & num_b :b.num){
                                if(blo_h.num.find(num_b.first)==blo_h.num.end()){
                                    blo_h.num[num_b.first]=num_b.second;
                                }
                                else{
                                    blo_h.num[num_b.first]+=num_b.second;
                                }
                                
                            }
                            int fes=1;
                            for(auto const & e: n_block){
                                if(e==blo_h) fes=0;
                            }
                            if(fes==1) n_block.push_back(blo_h);
                        }
                        
                    }
                }
            }
                
                    
                    if (n_block.size()==0) break;
                    
                    for(auto &e : n_block){
                        bool fess=1;
                        int ffg=1;
                        for(auto & g:inst.num){
                            if(e.num.find(g.first)!=e.num.end()){
                                if(e.num[g.first]>g.second) //std::cout<<"error"<<std::endl;
                                {ffg=0; break;}
                            }
                        }
                        if(ffg==0) continue;
                        for(auto &f:b_block){
                            if(e==f) {fess=0; break;}
                        }
                        if(fess==1) b_block.push_back(e);
                    }
                    
                    p_block=n_block;
                    
                }
                
        
    
    
    
    int h=(int)b_block.size()-1;
    while(h>=0){
        int fesi=1;
        for (auto &e : b_block[h].num) {
            if(e.second>inst.num[e.first])
            {fesi=0;break;}
        }
        if(fesi==0) b_block.erase(b_block.begin()+h);
        --h;
    }
    }
    */
}


void data::read_data(const std::string &fname_carton, const std::string &fname_product, const std::string &fname_order, const std::string &fname_benchmark){
    std::ifstream fin;
    std::string tmp;
    
    // read cartons
    fin.open(fname_carton.c_str());
    if(fin.fail()){
        std::cerr << "File open failed: " + fname_carton << std::endl;
    }
    std::string cid;
    while(fin >> cid){
        carton c;
        c.cid = cid;
        fin >> c.L >> c.W >> c.H >> tmp >> tmp;
        arr_carton.push_back(c);
    }
    fin.close();
    
    // read products
    fin.open(fname_product.c_str());
    if(fin.fail()){
        std::cerr << "File open failed: " + fname_product << std::endl;
    }
    int pno;
    while (fin >> pno) {
        double l,w,h;
        fin >> l >> w >> h;
        product p(pno, l, w, h);
        map_product[p.pno] = p;
    }
    fin.close();
    
    // read benchmark; each order is an instance of problem
    fin.open(fname_benchmark.c_str());
    if(fin.fail()){
        std::cerr << "File open failed: " + fname_benchmark << std::endl;
    }
    std::string oid;
    while(fin >> oid){
        problem inst;
        inst.oid = oid;
        fin >> inst.qty >> inst.benchmark_sol_carton >> inst.benchmark_sol_utl;
        // add order to map
        if(map_inst.find(inst.qty) == map_inst.end()){
            std::map<std::string, problem> m;
            m[inst.oid] = inst;
            map_inst[inst.qty] = m;
        }else{
            map_inst[inst.qty][inst.oid] = inst;
        }
    }
    fin.close();
    
    // read orders; discard orders not in benchmark
    fin.open(fname_order.c_str());
    if(fin.fail()){
        std::cerr << "File open failed: " + fname_order << std::endl;
    }
    while(fin >> oid){
        int line, pno; double _qty;
        fin >> line >> pno >> _qty;
        int qty = (int) (_qty + 0.5);
        for(auto & p : map_inst){
            if(p.second.find(oid) != p.second.end()){
                problem & inst = p.second[oid];
                if(inst.num.find(pno) == inst.num.end()){
                    inst.num[pno] = qty;
                }
                else{
                    inst.num[pno] += qty;
                }
            }
        }
    }
    fin.close();
}

bool operator < (const carton & a, const carton & b){
    return a.volume < b.volume;
}

std::ostream & operator<<(std::ostream & out, const carton & c){
    out << c.cid << "\t" << c.volume << "\t" << c.L << "\t" << c.W << "\t" << c.H;
    return out;
 }
