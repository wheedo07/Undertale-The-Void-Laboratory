#include "mainNode.h"
#include<godot_cpp/classes/scene_tree.hpp>
using namespace godot;
struct sleepFunction {
    function<void()> fun;
    double cool;
    double time;
    int id;
};
struct loopFunction {
    function<int(double delta)> fun;
    int id;
};

vector<sleepFunction> sleepFuns;
void MainNode::sleep(function<void()> fun, double cool, int id) {
    sleepFuns.push_back({fun, cool, 0, id});
}

vector<loopFunction> loopFuns;
void MainNode::loop(function<int(double delta)> fun, int id) {
    loopFuns.push_back({ fun, id });
}

int loop_count = 0;
static int next_sequence_id = 0;
void MainNode::sequence(vector<pair<function<void()>, LoopTime>> funs) {
    if(loop_count == 0) next_sequence_id = 0;
    loop_count++;

    int base_id = next_sequence_id;
    next_sequence_id += funs.size() + 1;
    int* index = new int(0);
    funs.insert(funs.begin(), {[](){}, 0.001});

    LoopTime looptime = funs[*index].second;
    if(holds_alternative<double>(looptime)) {
        sleep(funs[*index].first, std::get<double>(looptime), base_id + *index);
    }

    loop([this, funs, index, base_id](double delta) {
        if(*index < funs.size()) {
            bool is = false;
            for(auto& fun : sleepFuns) {
                if(fun.id == base_id + *index) is = true;
                if(is) break;
            }

            if(!is) for(auto fun : loopFuns) {
                if(fun.id == base_id + *index) is = true;
                if(is) break;
            }

            if(!is) {
                (*index)++;
                if(*index < funs.size()) {
                    LoopTime looptime = funs[*index].second;
                    if(holds_alternative<double>(looptime)) {
                        sleep(funs[*index].first, std::get<double>(looptime), base_id + *index);
                    }else executeTrue(std::get<function<int()>>(looptime), funs[*index].first, base_id + *index);
                }
            }
            return false;
        }else {
            loop_count--;
            delete index;
            return true;
        }
    });
}

void MainNode::executeTrue(function<int()> isFun, function<void()> fun, int id) {
    loop([this, fun, isFun](double delta) {
        if (isFun()) {
            fun();
            return true;
        }else return false;
    }, id);
}

void MainNode::time_loop(function<void(double delta)> fun, double time) {
    double* total_time = new double(0);
    loop([fun, time, total_time](double delta) {
        *total_time += delta;
        
        if(*total_time >= time) {
            delete total_time;
            return true;
        }

        fun(delta);
        return false;
    });
}

void MainNode::clear_system(bool is) {
    isClear = is;
    if(is) {
        Ref<SceneTreeTimer> timer = get_tree()->create_timer(clearReload_time);
        timer->connect("timeout", Callable(this, "clear_system").bind(false), CONNECT_ONE_SHOT);
    }
}

void MainNode::system(double delta) {
    if(isClear) {
        sleepFuns.clear();
        loopFuns.clear();
        loop_count = 0;
        next_sequence_id = 0;
    }else {
        int i1 = 0;
        for(auto& fun : sleepFuns) {
            if(fun.cool <= fun.time) {
                fun.fun();
                sleepFuns.erase(sleepFuns.begin() + i1);
            }else {
                fun.time+=delta;
                i1++;
            }
        }
        
        int i2 = 0;
        for(auto& fun : loopFuns) {
            if(fun.fun(delta)) 
                loopFuns.erase(loopFuns.begin() + i2);
            else i2++;
        }
    }
}