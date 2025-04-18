#include "trueLaboratory.h"
#include "engine/Overworld/Characters/enemy_overworld.h"
#include "env.h"

TrueLaboratory::TrueLaboratory() {}

TrueLaboratory::~TrueLaboratory() {}

void TrueLaboratory::_bind_methods() {
    ClassDB::bind_method(D_METHOD("flowey_move"), &TrueLaboratory::flowey_move);
}

void TrueLaboratory::ready() {
    String name = get_name();
    if(name == "true_1") {
        EnemyOverworld* flowey = Object::cast_to<EnemyOverworld>(get_node_internal("Flowey"));
        if(int(global->get_flag("main2")) == 1) {
            flowey->set_frame(1);
            camera->blind(0.5, 0.9);
            player->set_frame(1);
            
            sys->time_loop([this](double delta, TimeAccumPtr acc) {
                double* time = acc[0];
                *time += delta;
                
                Vector2 originalPos = player->get_position();
                if (*time >= 1.5 && *time < 10.0) {
                    float elapsedShakeTime = *time - 1.5;
                    float totalShakeTime = 8.5;
                    float progress = elapsedShakeTime / totalShakeTime;
                    
                    float amplitudeMultiplier = sin(progress * Math_PI); 
                    float amplitude = 2.5f * amplitudeMultiplier;
                    
                    float frequencyCurve;
                    if (progress < 0.7) {
                        frequencyCurve = progress * 1.4;
                    } else {
                        frequencyCurve = 0.98 - (progress - 0.7) * 0.5;
                    }
                    
                    float frequency = 1.5f + 5.5f * frequencyCurve;
                    
                    float offsetX = amplitude * sin(elapsedShakeTime * frequency * Math_PI);
                    
                    player->set_position(Vector2(originalPos.x + offsetX, originalPos.y));
                }
            }, 10);
            sys->sleep([this, flowey]() {
                player->set_frame(0);
                camera->blind(0.25, 0);
                flowey->connect("animation_finished", Callable(flowey, "hide"), CONNECT_ONE_SHOT);
                flowey->connect("animation_finished", Callable(this, "flowey_move"), CONNECT_ONE_SHOT);
                flowey->play_anim("dig");
            }, 10.2);
        }
    }
}

void TrueLaboratory::flowey_move() {
    if(int(global->get_flag("main2")) == 1) {
        EnemyOverworld* flowey = Object::cast_to<EnemyOverworld>(get_node_internal("Flowey"));
        flowey->set_position(Vector2(player->get_position().x, -27));
        flowey->show();
        flowey->play_anim("dig", 0.5, true);
        flowey->connect("animation_finished", Callable(flowey, "set_frame").bind(0), CONNECT_ONE_SHOT);
        flowey->connect("animation_finished", Callable(player, "force_direction").bind(Vector2(0, -1)), CONNECT_ONE_SHOT);
        sys->sleep([this, flowey]() {
            summontextbox()->character(Character::FLOWEY, sys->dia()->from(
                PackedStringArray({
                    String::utf8("* 테스트")
                })
            ));
        }, 1.3);
    }
}