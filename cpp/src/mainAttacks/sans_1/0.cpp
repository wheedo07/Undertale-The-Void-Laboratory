#include "attack_sans1.h"
#include "env.h"

void AttackSans1::turn_0() {
    emit_signal("throws", Vector2(0, 1), 75);

    BoneSpike* spike = create_boneSpike(Masking::ABSOLUTE_CLIP);
    spike->set_position(Vector2(255, 350));
    spike->fire(Vector2(300, 100), 0.7, 1.5);
    sys->sequence({
        {[this]() {
            soul->set_mode();
        }, 1.4f},
        {[this]() {
            sys->time_loop([this](double delta, TimeAccumPtr acc) {
                double* time = acc[0];
                double* wave_time = acc[1];

                *wave_time += delta;
                if(*time >= 0.15) {
                    double base_wave = sin(*wave_time * Math_PI) * cos(*wave_time * 0.5 * Math_PI);
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
        }, 8.2f},
        {[this]() {
            Blaster* bl = create_blaster(Masking::ABSOLUTE);
            bl->set_position(Vector2(265, -100));
            bl->fire(Vector2(265, 100), 1.5, 0.3, 0.5);
        }, 0.2},
        {[this]() {
            soul->set_mode();
            box->change_size(Vector2(200, 140));

            Blaster* bl = create_blaster(Masking::ABSOLUTE);
            bl->set_position(Vector2(100, -100));
            bl->set_rotation_degrees(-37.2);
            bl->fire(Vector2(100, 100), 1.5, 0.3, 0.7);

            Blaster* bl2 = create_blaster(Masking::ABSOLUTE);
            bl2->set_position(Vector2(520, -100));
            bl2->set_rotation_degrees(35);
            bl2->fire(Vector2(520, 100), 1.5, 0.3, 0.7);
        }, 1.0f},
        {[this]() {
            sys->time_loop([this](double delta, TimeAccumPtr acc) {
                double* time = acc[0];
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
        }, 0.3},
        {[this]() {
            Bone* b = create_bone(Masking::ABSOLUTE_CLIP);
            b->set_rotation_degrees(180);
            b->set_position(Vector2(120, 400));
            b->set_mode(Bullet::MODE_BLUE);
            b->fire(Vector2(420, 400), Bullet::MOVEMENT_TWEEN, 160);
            b->tween_height(150, 0);
            
            Bone* b2 = create_bone(Masking::ABSOLUTE_CLIP);
            b2->set_rotation_degrees(180);
            b2->set_position(Vector2(520, 400));
            b2->set_mode(Bullet::MODE_BLUE);
            b2->fire(Vector2(220, 400), Bullet::MOVEMENT_TWEEN, 160);
            b2->tween_height(150, 0);
            sys->sleep([this, b, b2]() { b->queue_free(); b2->queue_free(); }, 1.8);
        }, 10.2},
        {[this]() {
            box->change_size(Vector2(220, 160));
            Vector2 center = Vector2(320, 325);
            float radius = 130.0f;
            float angle_increment = 90.0f;
            sys->time_loop([this, center, radius, angle_increment](double delta, TimeAccumPtr acc) {
                double* time = acc[0];
                double* current_angle = acc[1];
              
                *current_angle += UtilityFunctions::deg_to_rad(angle_increment) * delta;
                *current_angle = UtilityFunctions::fmod(*current_angle, 2.0 * Math_PI);
                if (*time >= 0.2) {
                    Vector2 spawn_position = Vector2(center.x + radius * cos(*current_angle), center.y + radius * sin(*current_angle));
                    Blaster* bl = create_blaster(Masking::ABSOLUTE);
                    bl->set_position(Vector2(spawn_position.x, spawn_position.y-50));
                    bl->set_rotation_degrees(UtilityFunctions::rad_to_deg(*current_angle) + 90);
                    bl->fire(spawn_position, 1.5, 0.3, 0.4);
                    *time = 0;
                }else *time += delta;
            }, 9);
        }, 1.5}
    });
}