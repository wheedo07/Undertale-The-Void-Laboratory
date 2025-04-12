#ifndef LaboratoryNode_H
#define LaboratoryNode_H
#include "engine/Overworld/overworld.h"
#include<godot_cpp/classes/node2d.hpp>
#include<godot_cpp/classes/camera2d.hpp>
namespace godot {
    class LaboratoryNode : public Overworld {
        GDCLASS(LaboratoryNode, Overworld);
        
        protected:
            static void _bind_methods();

        private:
            AudioStreamPlayer* event_sound;
            AudioStreamPlayer* event_main2;
            Node2D* gaster;
            Sprite2D* special_1;
            Ref<Tween> tween;

            void character_talk();
            void finished_on();

        public:
            LaboratoryNode();
            ~LaboratoryNode();

            void ready() override;
            void init();
    };
}

#endif