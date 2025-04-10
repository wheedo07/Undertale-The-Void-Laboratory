#include "mainNode.h"
#include<godot_cpp/classes/scene_tree.hpp>
using namespace godot;
struct sleepFunction {
    function<void()> fun;
    double cool;
    double time;
};

vector<sleepFunction> sleepFuns;
void MainNode::sleep(function<void()> fun, double cool) {
    sleepFuns.push_back({fun, cool, 0});
}

vector<function<int(double delta)>> loopFuns;
int isLoop = false;
void MainNode::loop(function<int(double delta)> fun) {
    loopFuns.push_back(fun);
    isLoop = true;
}

void MainNode::sequence(function<int()> isFun, vector<function<void()>> funs) {
    loop([this, funs, isFun](double delta) {
        static size_t current_index = 0;
        
        if (isFun()) {
            if (current_index < funs.size()) {
                funs[current_index]();
                current_index++;
                return false;
            }
            current_index = 0;  // 리셋
            return true;  // 모든 액션 완료
        }else return false;
    });
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
        for(int i=0; i < sleepFuns.size(); i++) 
            sleepFuns.erase(sleepFuns.begin() + i);
        for(int i=0; i < loopFuns.size(); i++) 
            loopFuns.erase(loopFuns.begin() + i);
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
            if(fun(delta)) 
            loopFuns.erase(loopFuns.begin() + i2);
            else i2++;
        }
    }
}