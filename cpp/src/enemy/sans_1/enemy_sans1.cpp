#include "enemy_sans1.h"
#include "src/mainNode/dropText.h"
#include "env.h"

Enemy_SANS1::Enemy_SANS1() {}

Enemy_SANS1::~Enemy_SANS1() {}

void Enemy_SANS1::_bind_methods() {
    ClassDB::bind_method(D_METHOD("_on_throws", "dir", "power"), &Enemy_SANS1::_on_throws, DEFVAL(Vector2(0, 1)), DEFVAL(10));
    ClassDB::bind_method(D_METHOD("end_throws"), &Enemy_SANS1::end_throws);
}

void Enemy_SANS1::ready() {
    body = Object::cast_to<AnimatedSprite2D>(get_node_internal("sans/Idle/body"));
    leg = Object::cast_to<AnimatedSprite2D>(get_node_internal("sans/Idle/leg"));
    head = Object::cast_to<AnimatedSprite2D>(get_node_internal("sans/Idle/body/head"));
    throw_head = Object::cast_to<AnimatedSprite2D>(get_node_internal("sans/Throw/head"));
    AnimStates = Object::cast_to<AnimationNodeStateMachinePlayback>(get_node_internal("sans/States")->get("parameters/playback"));
    throw_timer = Object::cast_to<Timer>(get_node_internal("Timer"));
    sprite = Object::cast_to<Node2D>(get_sprites());
    attackScene = ResourceLoader::get_singleton()->load("res://Game/main_attacks.tscn");

    sprite->set_z_index(102);
    main->back_scene->set_visible(true);
    sprite->set_y_sort_enabled(true);
}

void Enemy_SANS1::_on_get_turn() {
    if(main->turn_number == 0) {
        if(global->get_flag("sans_1_death")) {
            global->get_Music()->seek(22);
            body->set_frame(8);
            leg->set_frame(2);
            head->set_frame(20);
            camera_pro(2, "zoom", Vector2(2,2));
            camera_pro(2, "rotation", 0.6);
            camera_pro(0.1, "position", Vector2(320, 150));
            sys->sequence({
                {[this]() {
                    camera_pro(0.5);
                    head->set_frame(21);
                    body->set_frame(9);
                    leg->set_frame(3);
                    main->back_scene->set_visible(false);
                    sprite->set_z_index(0);
                    sprite->set_y_sort_enabled(false);
                    
                    create_attack()->set_part(PartType::sans_1);
                    attacks->start_attack(0);
                }, 3.0f},
                {[this]() {
                    head->set_frame(22);
                    body->set_frame(10);
                    leg->set_frame(0);
                }, 24.0f},
                {[this]() {
                    play_dialogue(0);
                }, 11.0f},
                {[this]() {
                    head->set_frame(3);
                    body->set_frame(0);
                    attacks->end_attack(0);
                }, [this]() { return !global->get_battle_text_box(); }}
            });
        }else {
            Node2D* friends = Object::cast_to<Node2D>(get_node_internal("friends"));
            DropText* drop = Object::cast_to<DropText>(get_node_internal("DropText"));
            head->set_frame(4);
            auto isFun = [this]() { return !global->get_battle_text_box(); };
            sys->sequence({
                {[this, friends]() {
                    play_dialogue(0, 2.5, false);
                    friends->set_visible(true);
                    friends->set_modulate(Color(1, 1, 1, 0)); 
                    Tween* tween = Object::cast_to<Tween>(create_tween().ptr());
                    tween->tween_property(friends, "modulate:a", 1, 8.0);
                }, 2.0f},
                {[this, friends]() {
                    head->set_frame(17);
                    body->set_frame(8);
                    leg->set_frame(2);
                    play_dialogue(1, 2.1, false);
                    Tween* tween = Object::cast_to<Tween>(create_tween().ptr());
                    tween->tween_property(friends, "modulate:a", 0.0, 5.0);
                    tween->connect("finished", Callable(friends, "queue_free"));
                }, isFun},
                {[this, drop]() {
                    head->set_frame(20);
                    drop->set_font_size(30);
                    drop->crumble_text(String::utf8("지 옥 에 나 떨 어 져"), Vector2(-130, 0));
                }, isFun},
                {[this, drop]() {
                    drop->start();
                    camera_pro(0.5);
                    head->set_frame(21);
                    body->set_frame(9);
                    leg->set_frame(3);
                    main->back_scene->set_visible(false);
                    sprite->set_z_index(0);
                    sprite->set_y_sort_enabled(false);
                    
                    create_attack()->set_part(PartType::sans_1);
                    attacks->start_attack(0);
                }, 3.0f},
                {[this]() {
                    head->set_frame(22);
                    body->set_frame(10);
                    leg->set_frame(0);
                }, 24.0f},
                {[this]() {
                    play_dialogue(0);
                }, 11.0f},
                {[this]() {
                    head->set_frame(3);
                    body->set_frame(0);
                    attacks->end_attack(0);
                }, isFun}
            });
        }
    }else if(main->turn_number == 1) {
        soul->set_mode();
        auto isFun = [this]() { return !global->get_battle_text_box(); };
        sys->sequence({
            {[this]() {
                play_dialogue(1);
            }, 0.1f},
            {[this]() {
                global->get_scene_container()->get_camera()->Void(0, 5, 0.005, 0.1, 2);
                audio_player->play("beep");
                play_dialogue(2);
            }, isFun},
            {[this]() {
                audio_player->stop_audio("beep");
                global->set_flags("main2", 1);
                scene_changer->load_cached_overworld_scene();
            }, isFun}
        });
    }
}

void Enemy_SANS1::on_death_player() {
    global->save_flag("sans_1_death", true);
}

MainAttacks* Enemy_SANS1::create_attack() {
    MainAttacks* attack = Object::cast_to<MainAttacks>(attacks->add_attack(attackScene));
    attack->connect("throws", Callable(this, "_on_throws"));
    return attack;
}

void Enemy_SANS1::_on_throws(Vector2 dir, int power) {
    throw_timer->start();
    AnimStates->stop();
    String key = "";
    if(dir == Vector2(-1, 0))
        key = "throw_left";
    else if(dir == Vector2(1, 0))
        key = "throw_right";
    else if(dir == Vector2(0, 1))
        key = "throw_down";
    else if(dir == Vector2(0, -1))
        key = "throw_up";

    AnimStates->travel(key); 
    throw_head->play("crazy", true);

    sys->sleep([this, dir, power]() {
        soul->set_gravity_direction(dir);
        soul->set_gravity_multiplier(power);
    }, 0.2);
}

void Enemy_SANS1::end_throws() {
    throw_head->set_animation("heads");
}

void Enemy_SANS1::camera_pro(float time, String key, Variant value) {
    Tween* tween = Object::cast_to<Tween>(create_tween().ptr());
    // zoom, position, rotation
    if(key == "all") {
        tween->tween_property(main->camera, "zoom", Vector2(1,1), time);
        tween->tween_property(main->camera, "position", Vector2(320, 240), time);
        tween->tween_property(main->camera, "rotation", 0, time);
    }else tween->tween_property(main->camera, key, value, time);
}