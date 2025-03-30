#ifndef MainAttacks_H
#define MainAttacks_H

#include "engine/Battle/Attacks/attacks.h"
#include "engine/Battle/Bullets/bone.h"
#include "engine/Battle/Bullets/blaster.h"
enum PartType {
    sans_1
};

namespace godot {
    class MainAttacks : public AttackBase {
        GDCLASS(MainAttacks, AttackBase);

        protected:
            static void _bind_methods();

        private:
            PartType part;
            Ref<PackedScene> bone;
            Ref<PackedScene> blaster;

        public:
            MainAttacks();
            ~MainAttacks();

            void ready() override;
            void start_attack() override;
            void set_part(PartType type);

            void sans_1();

            Bone* create_bone(Masking type);
            Blaster* create_blaster(Masking type);
    };
}

#endif