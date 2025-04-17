#include "mainAttacks.h"
#include "env.h"
#include<memory>

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
    // double* time = new double(1);
    // sys->time_loop([this, time](double delta) {
    //     if(*time >= 1.5) {
    //         Bone* b = create_bone(Masking::ABSOLUTE);
    //         b->set_rotation_degrees(180);
    //         b->set_position(Vector2(120, 400));
    //         b->fire(Vector2(420, 400), Bullet::MOVEMENT_TWEEN, 160)->tween_height(70, 1.1);

    //         Bone* b2 = create_bone(Masking::ABSOLUTE);
    //         b2->set_rotation_degrees(180);
    //         b2->set_position(Vector2(520, 400));
    //         b2->fire(Vector2(220, 400), Bullet::MOVEMENT_TWEEN, 160)->tween_height(70, 1.1);
    //         sys->sleep([this, b, b2]() { b->queue_free(); b2->queue_free(); }, 1.8);
    //         *time = 0;
    //     }else *time += delta;
    // }, 10);
    // sys->sleep([this]() {
    //     soul->set_mode(SoulBattle::RED);
    //     box->change_size(Vector2(200, 140));
    //     double* time = new double(1);
    //     sys->time_loop([this, time](double delta) {
    //         if(*time >= 1.5) {
    //             Blaster* bl = create_blaster(Masking::ABSOLUTE);
    //             Vector2 Sv = soul->get_position();
    //             bl->set_position(Vector2(Sv.x, -100));
    //             bl->fire(Vector2(Sv.x, 100), 1.5, 0.3, 0.7);

    //             Bone* b1 = create_bone(Masking::RELATIVE_BOX_CLIP);
    //             b1->set_rotation_degrees(-90);
    //             b1->set_position(Vector2(0, 0));
    //             b1->tween_height(110, 0);
    //             Vector2 pos1 = b1->get_global_position();
    //             pos1.y += 200;
    //             b1->fire(pos1, Bullet::MOVEMENT_TWEEN, 85);

    //             Bone* b2 = create_bone(Masking::RELATIVE_BOX_CLIP);
    //             b2->set_rotation_degrees(90);
    //             b2->set_position(Vector2(200, 140));
    //             b2->tween_height(110, 0);
    //             Vector2 pos2 = b2->get_global_position();
    //             pos2.y -= 200;
    //             b2->fire(pos2, Bullet::MOVEMENT_TWEEN, 85);

    //             sys->sleep([this, b1, b2]() { b1->queue_free(); b2->queue_free(); }, 1.3);
    //             *time = 0;
    //         }else *time += delta;
    //     }, 10);
    //     sys->sleep([this]() {
    //         Bone* b = create_bone(Masking::ABSOLUTE_CLIP);
    //         b->set_rotation_degrees(180);
    //         b->set_position(Vector2(120, 400));
    //         b->set_mode(Bullet::MODE_BLUE);
    //         b->fire(Vector2(420, 400), Bullet::MOVEMENT_TWEEN, 160);
    //         b->tween_height(150, 0);

    //         Bone* b2 = create_bone(Masking::ABSOLUTE_CLIP);
    //         b2->set_rotation_degrees(180);
    //         b2->set_position(Vector2(520, 400));
    //         b2->set_mode(Bullet::MODE_BLUE);
    //         b2->fire(Vector2(220, 400), Bullet::MOVEMENT_TWEEN, 160);
    //         b2->tween_height(150, 0);
    //         sys->sleep([this, b, b2]() { b->queue_free(); b2->queue_free(); }, 1.8);
    //     }, 10.5);
    // }, 10.1);
    // sys->sleep([this]() {
    //     box->change_size(Vector2(430, 210));
    //     double* time = new double(2);
    //     sys->time_loop([this, time](double delta) {
    //         if(*time >= 2.2) {
    //             int num_blasters = UtilityFunctions::randi_range(2, 4);
    //             for(int i = 0; i < num_blasters; i++) {
    //                 Blaster* bl = create_blaster(Masking::ABSOLUTE);
    //                 float angle = i * (Math_PI / 4.0); 
    //                 Vector2 center = soul->get_position();
                    
    //                 Vector2 pos = center + Vector2(cos(angle), sin(angle)) * 200;
    //                 bl->set_position(pos);
    //                 bl->set_rotation(angle + Math_PI);  
                    
    //                 bl->fire(center, 1.5, 0.5, 1.3);
    //             }
    //             *time = 0;
    //         }else *time += delta;
    //     }, 10);
    // }, 24.5);
}