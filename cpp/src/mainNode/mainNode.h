#ifndef MainNode_H
#define MainNode_H
#include "engine/defaultNode/scene_container.h"
#include "engine/resources/Dialogues/dialogues.h"
#include<godot_cpp/classes/resource_loader.hpp>
#include<functional>
#include<variant>
using namespace std;
#define clearReload_time 1.3f
using LoopTime = std::variant<double, function<int()>>;
using TimeAccumPtr = double (*)[3];
namespace godot {
    class MainNode : public SceneContainer {
        GDCLASS(MainNode, SceneContainer);

        protected:
            static void _bind_methods();

        private:
            ResourceLoader* loader;
            bool isClear;
        
        public:
            MainNode();
            ~MainNode();

            void _ready() override;
            void _process(double delta) override;

            void clear_system(bool is=true);
            void system(double delta);
            void sleep(function<void()> fun, double cool, int id=-1);
            void loop(function<bool(double delta, TimeAccumPtr acc)> fun, int id=-1);
            void time_loop(function<void(double delta, TimeAccumPtr acc)> fun, double duration);
            void executeTrue(function<int()> isFun, function<void()> fun, int id=-1);
            void sequence(vector<pair<function<void()>, LoopTime>> funs);

            Ref<Dialogues> dia();
            void load_battle(String path, Vector2 vec = Vector2(48, 452));
    };
}

#endif