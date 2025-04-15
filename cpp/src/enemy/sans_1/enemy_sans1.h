#ifndef Enemy_SANS1_H
#define Enemy_SANS1_H

#include "engine/Battle/enemy.h"
#include "engine/Battle/battle_system.h"
#include "src/mainAttacks/mainAttacks.h"
namespace godot {
    class Enemy_SANS1 : public Enemy {
        GDCLASS(Enemy_SANS1, Enemy);

        protected:
            static void _bind_methods();
        
        private:
            AnimatedSprite2D* body;
            AnimatedSprite2D* leg;
            AnimatedSprite2D* head;
            AnimatedSprite2D* throw_head;
            Timer* throw_timer;
            Node2D* sprite;
            AnimationNodeStateMachinePlayback* AnimStates;

            Ref<PackedScene> attackScene;

        public:
            Enemy_SANS1();
            ~Enemy_SANS1();

            void ready() override;
            void _on_get_turn() override;
            void on_death_player() override;

            MainAttacks* create_attack();
            void camera_pro(float time, String key="all", Variant value=true);
            void _on_throws(Vector2 dir=Vector2(0, 1), int power = 10);
            void end_throws();
    };
}

#endif