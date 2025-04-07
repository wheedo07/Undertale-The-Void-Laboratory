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
            bool is;

            void character_talk();

        public:
            LaboratoryNode();
            ~LaboratoryNode();

            void ready() override;
            void init();
    };
}

#endif