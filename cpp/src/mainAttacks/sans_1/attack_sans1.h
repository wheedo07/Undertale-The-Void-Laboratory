#ifndef AttackSans1_H
#define AttackSans1_H

#include "engine/Battle/Attacks/attacks.h"
#include "engine/Battle/Bullets/bone.h"
#include "engine/Battle/Bullets/bone_spike.h"
#include "engine/Battle/Bullets/blaster.h"
namespace godot {
    class AttackSans1 : public AttackBase {
        GDCLASS(AttackSans1, AttackBase);

        protected:
            static void _bind_methods();

        private:
            Ref<PackedScene> bone;
            Ref<PackedScene> bone_spike;
            Ref<PackedScene> blaster;

            void turn_0();

        public:
            AttackSans1();
            ~AttackSans1();

            void ready() override;
            void start_attack() override;

            Bone* create_bone(Masking type);
            BoneSpike* create_boneSpike(Masking type);
            Blaster* create_blaster(Masking type);
    };
}

#endif