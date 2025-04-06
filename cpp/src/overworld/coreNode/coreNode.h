#ifndef CoreNode_H
#define CoreNode_H
#include "engine/Overworld/overworld.h"
#include<godot_cpp/classes/node2d.hpp>
#include<godot_cpp/classes/camera2d.hpp>
namespace godot {
    class CoreNode : public Overworld {
        GDCLASS(CoreNode, Overworld);

        protected:
            static void _bind_methods();

        private:
            void main2_selected_option(int option);
        
        public:
            CoreNode();
            ~CoreNode();

            void ready() override;

            void voidWall();
            void event1();
            void event2();
            void text_trigger1();
            void main2_start();
    };
}

#endif