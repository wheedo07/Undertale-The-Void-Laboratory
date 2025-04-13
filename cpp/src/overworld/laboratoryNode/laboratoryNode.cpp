#include "laboratoryNode.h"
#include "env.h"
#include "engine/Overworld/Characters/enemy_overworld.h"

LaboratoryNode::LaboratoryNode() {}

LaboratoryNode::~LaboratoryNode() {}

void LaboratoryNode::_bind_methods() {
    ClassDB::bind_method(D_METHOD("init"), &LaboratoryNode::init);
    ClassDB::bind_method(D_METHOD("character_talk"), &LaboratoryNode::character_talk);
    ClassDB::bind_method(D_METHOD("main2_event"), &LaboratoryNode::main2_event);
    ClassDB::bind_method(D_METHOD("finished_on"), &LaboratoryNode::finished_on);
}

void LaboratoryNode::ready() {
    event_sound = Object::cast_to<AudioStreamPlayer>(get_node_internal("event/sound"));
    event_main2 = Object::cast_to<AudioStreamPlayer>(get_node_internal("event/main2"));
    camera->blind(0.3);
    camera->connect("finished_tween", Callable(this, "init"), CONNECT_ONE_SHOT);
}

void LaboratoryNode::init() {
    EnemyOverworld* sans = Object::cast_to<EnemyOverworld>(get_node_internal("Sans"));

    if(!global->get_flag("main2")) {
        global->set_player_move(false);
        sans->start_walking(Vector2(0, -1));
        sans->start_walking();
        
        player->start_walking(Vector2i(-1, 0));
        auto isFun = [this]() { return !global->get_player_text_box(); };
        sys->sequence({
            {[this, sans]() {
                player->start_walking();
                player->set_walk_speed(80);

                sans->start_walking(Vector2i(1, 0));
                sans->start_walking();
                sans->show_alert();
            }, 1.5f},
            {[this]() {
                if(global->get_flag("sans_1_death")) {
                    summontextbox()->character(Character::SANS, sys->dia()->from(
                        PackedStringArray({
                            String::utf8("* 헤헤"),
                            String::utf8("* 계속해볼까?"),
                        })
                    )->set_expressions(Array::make(1, 5)));
                }else summontextbox()->character(Character::SANS, sys->dia()->from(
                    PackedStringArray({
                        String::utf8("* ..?"),
                        String::utf8("* ..."),
                        String::utf8("* 음 kid 여기 있었네.."),
                        String::utf8("* 뼈밖에서 보니까 반갑네.."),
                        String::utf8("* 내가 여기 있는것도 우연은 아닐거고... 그치?"),
                    })
                )->set_expressions(Array::make(10, 18, 0, 1, 4))
                ->set_speed(Array::make(0.03)));
            }, 0.4f},
            {[this]() {
                if(global->get_flag("sans_1_death")) return;
                summontextbox()->generic(sys->dia()->from(
                    PackedStringArray({
                        String::utf8("* (연구소는 평소와 다르게 음산하다...)")
                    })
                ));
            }, isFun},
            {[this]() {
                if(global->get_flag("sans_1_death")) return;
                summontextbox()->character(Character::SANS, sys->dia()->from(
                    PackedStringArray({
                        String::utf8("* 다른 타임라인에서 네가 한 일들..."),
                        String::utf8("* 얼마나 많은 괴물들을 죽였지?"),
                        String::utf8("* 얼마나 많은 타임라인을 망쳐놨지?"),
                        String::utf8("* 나는 기억하고 있어 너가 수많은 리셋으로 이르킨 일들.."),
                        String::utf8("* ..."),
                        String::utf8("* 그렇기에 kid 한가지 조언을 할게 너를 위해서기도 해"),
                        String::utf8("* 이제 그만두는게 좋을거야")
                    })
                )->set_expressions(Array::make(4, 5, 15, 17, 19, 4, 5))
                ->set_speed(Array::make(0.03, 0.01, 0.05)));
            }, isFun},
            {[this]() {
                sys->load_battle("res://Game/encounters/sans_1.tres",  Vector2(324, 323));
            }, isFun}
        });
    }else {
        summontextbox()->character(Character::SANS, sys->dia()->from(
            PackedStringArray({
                String::utf8("* ..."),
                String::utf8("* 잠깐 있어봐"),
            })
        )->set_expressions(Array::make(2, 18)));
        sys->executeTrue([this]() { return !global->get_player_text_box(); },
        [this, sans]() {
            audio_player->play("teleport");
            camera->blind(0.2, 1, 0.3);
            sans->set_position(Vector2(-420, 233));
            sans->start_walking(Vector2i(0, -1));
            sans->start_walking();
            camera->connect("finished_tween", Callable(global, "set_player_move").bind(true), CONNECT_ONE_SHOT);
            camera->connect("finished_tween", Callable(camera, "Void"), CONNECT_ONE_SHOT);
        });
    }
}

void LaboratoryNode::character_talk() {
    camera->Void(0, 5, 0.005, 0.1, 2);
    audio_player->play("beep");
    TextBox* textbox = summontextbox();
    textbox->set_key(false);
    textbox->character(Character::SANS, sys->dia()->from(
        PackedStringArray({
            String::utf8("어..? 잠깐만.. 뭔가 이상한..")
        })
    )->set_speed(Array::make(0.2))->set_expressions(Array::make(22)));
    sys->sleep([this, textbox]() { textbox->on_text_click_played(); }, 3.8);
    sys->executeTrue([this]() { return !global->get_player_text_box(); },
    [this]() {
        audio_player->stop_audio("beep");
        global->set_player_in_menu(true);
        camera->blind(0, 1, 3.5);
        camera->connect("finished_tween", Callable(this, "main2_event"), CONNECT_ONE_SHOT);
    });
}

void LaboratoryNode::main2_event() {
    audio_player->loop_play("glitch");
    camera->glitch(4, 0.1);

    event_main2->play();
    Ref<PackedScene> scene = ResourceLoader::get_singleton()->load("res://Game/encounters/body/gaster.tscn");
    gaster = Object::cast_to<Node2D>(scene->instantiate());
    camera->blinder->add_child(gaster);
    gaster->set_position(Vector2(330, 200));
    sys->sequence({
        {[this]() {
            gaster->get_node_internal("head")->call("set_frame", 1);
            camera->glitch(0.15, 0.4);
        }, 4.0f},
        {[this]() {
            gaster->queue_free();
            camera->Void(0, 5, 0.02, 0.1, 3);
            special_1 = Object::cast_to<Sprite2D>(get_node_internal("event/special_1")->duplicate());
            camera->blinder->add_child(special_1);
            special_1->set_visible(true);
            special_1->set_position(Vector2(400, 50));
            special_1->set_modulate(Color(1, 1, 1, 0.0));
            special_1->set_scale(Vector2(0.1, 0.1));

            tween = create_tween()->set_parallel();
            tween->tween_property(special_1, "scale", Vector2(1.5, 1.5), 7.0)->set_ease(Tween::EASE_OUT)->set_trans(Tween::TRANS_CUBIC);
            tween->tween_property(special_1, "modulate", Color(1, 1, 1, 1.0), 3.0)->set_ease(Tween::EASE_IN)->set_trans(Tween::TRANS_SINE);
        }, 3.0f},
        {[this]() {
            event_sound->connect("finished", Callable(this, "finished_on"), CONNECT_ONE_SHOT);
            event_main2->stop();
            event_sound->play();
            camera->glitch(0, 0.8);
        }, 3.2f}
    });
}

void LaboratoryNode::finished_on() {
    if(tween->is_valid()) tween->kill();
    special_1->queue_free();
    camera->kill();
    audio_player->stop_audio("glitch");
    global->set_player_in_menu(false);
    printf("end\n");
}