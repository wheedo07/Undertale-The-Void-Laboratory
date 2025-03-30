#ifndef MainNode_H
#define MainNode_H
#include "engine/defaultNode/scene_container.h"
#include "engine/resources/Dialogues/dialogues.h"
#include<godot_cpp/classes/resource_loader.hpp>
#include<functional>
using namespace std;
namespace godot {
    class MainNode : public SceneContainer {
        GDCLASS(MainNode, SceneContainer);

        protected:
            static void _bind_methods();

        private:
            ResourceLoader* loader;
        
        public:
            MainNode();
            ~MainNode();

            void _ready() override;
            void _process(double delta) override;

            void clear_system();
            void system(double delta);
            void sleep(function<void()> fun, double cool);
            void loop(function<int(double delta)> fun);
            void time_loop(function<void(double delta)> fun, double time);
            void sequence(function<int()> isFun, vector<function<void()>> funs);

            Ref<Dialogues> dia();
            void load_battle(String path, Vector2 vec = Vector2(48, 452));
    };
}

#endif