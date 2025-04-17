#include "mainAttacks.h"
#include "env.h"
#include<chrono>

double get_time() {
    using namespace std::chrono;
    static auto start_time = high_resolution_clock::now();
    auto current_time = high_resolution_clock::now();
    duration<double> elapsed = current_time - start_time;
    return elapsed.count();
}

void MainAttacks::sans_1() {
    emit_signal("throws", Vector2(0, 1), 70);

    BoneSpike* spike = create_boneSpike(Masking::ABSOLUTE_CLIP);
    spike->set_position(Vector2(255, 350));
    spike->fire(Vector2(300, 100), 0.7, 1.5);
    sys->sequence({
        {[this]() {
            soul->set_mode(SoulBattle::RED);
        }, 1.4f},
        {[this]() {
            sys->time_loop([this](double delta, double* time) {
                if(*time >= 0.15) {
                    double current_time = get_time();
                    double base_wave = sin(current_time * 2.0 * Math_PI);
                    double wave_height_1 = 70 + base_wave * 30;
                    double wave_height_2 = 140 - wave_height_1;
                    
                    Bone* b = create_bone(Masking::ABSOLUTE_CLIP);
                    b->set_rotation_degrees(180);
                    b->set_position(Vector2(90, 400));
                    b->fire(Vector2(420, 400), Bullet::MOVEMENT_VELOCITY, 200)->tween_height(wave_height_1, 0.3);
                    
                    Bone* b2 = create_bone(Masking::ABSOLUTE_CLIP);
                    b2->set_position(Vector2(90, 250));
                    b2->fire(Vector2(420, 250), Bullet::MOVEMENT_VELOCITY, 200)->tween_height(wave_height_2, 0.3);
                    
                    sys->sleep([this, b, b2]() { b->queue_free(); b2->queue_free(); }, 1.8);
                    *time = 0;
                }else *time += delta;
            }, 7);
        }, 1.0f},
        {[this]() {
            emit_signal("throws", Vector2(-1, 0));
        }, 7.1},
        {[this]() {
            Blaster* bl = create_blaster(Masking::ABSOLUTE);
            bl->set_position(Vector2(265, -100));
            bl->fire(Vector2(265, 100), 1.5, 0.3, 0.5);
        }, 0.2},
        {[this]() {
            soul->set_mode(SoulBattle::RED);
            box->change_size(Vector2(200, 140));
            sys->time_loop([this](double delta, double* time) {
                if(*time >= 1.5) {
                    Blaster* bl = create_blaster(Masking::ABSOLUTE);
                    Vector2 Sv = soul->get_position();
                    bl->set_position(Vector2(Sv.x, -100));
                    bl->fire(Vector2(Sv.x, 100), 1.5, 0.3, 0.7);
                    
                    Bone* b1 = create_bone(Masking::RELATIVE_BOX_CLIP);
                    b1->set_rotation_degrees(-90);
                    b1->set_position(Vector2(0, 0));
                    b1->tween_height(110, 0);
                    Vector2 pos1 = b1->get_global_position();
                    pos1.y += 200;
                    b1->fire(pos1, Bullet::MOVEMENT_TWEEN, 85);
                    
                    Bone* b2 = create_bone(Masking::RELATIVE_BOX_CLIP);
                    b2->set_rotation_degrees(90);
                    b2->set_position(Vector2(200, 140));
                    b2->tween_height(110, 0);
                    Vector2 pos2 = b2->get_global_position();
                    pos2.y -= 200;
                    b2->fire(pos2, Bullet::MOVEMENT_TWEEN, 85);
                    
                    sys->sleep([this, b1, b2]() { b1->queue_free(); b2->queue_free(); }, 1.3);
                    *time = 0;
                }else *time += delta;
            }, 10);
        }, 1.0f}
    });
}