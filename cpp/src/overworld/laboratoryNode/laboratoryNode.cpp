#include "laboratoryNode.h"
#include "env.h"
#include "engine/Overworld/Characters/enemy_overworld.h"

LaboratoryNode::LaboratoryNode() {
    is = false;
}

LaboratoryNode::~LaboratoryNode() {}

void LaboratoryNode::_bind_methods() {
    ClassDB::bind_method(D_METHOD("init"), &LaboratoryNode::init);
    ClassDB::bind_method(D_METHOD("character_talk"), &LaboratoryNode::character_talk);
}

void LaboratoryNode::ready() {
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
        sys->sleep([this]() { is = true; }, 1.5);
        sys->sequence([this]() { return is && !global->get_player_text_box(); }, {
            [this, sans]() {
                is = false;
                player->start_walking();
                player->set_walk_speed(80);
                
                sans->start_walking(Vector2i(1, 0));
                sans->start_walking();
                sans->show_alert();
                sys->sleep([this]() { is = true; }, 0.4);
            },
            [this]() {
                summontextbox()->character(Character::SANS, sys->dia()->from(
                    PackedStringArray({
                        String::utf8("* ..?"),
                        String::utf8("* ..."),
                        String::utf8("* 음 kid 여기 있었네.."),
                        String::utf8("* 뼈밖에서 보니까 반갑네.."),
                        String::utf8("* 내가 여기 있는것도 우연은 아닐거고... 그치?"),
                    })
                )->set_expressions(Array::make(10, 18, 0, 1, 4))
                ->set_speed(Array::make(0.03)));
            },
            [this]() {
                summontextbox()->generic(sys->dia()->from(
                    PackedStringArray({
                        String::utf8("* (연구소는 평소와 다르게 음산하다...)")
                    })
                ));
            },
            [this]() {
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
            },
            [this]() {
                sys->load_battle("res://Game/encounters/sans_1.tres",  Vector2(324, 323));
            }
        });
    }else {
        summontextbox()->character(Character::SANS, sys->dia()->from(
            PackedStringArray({
                String::utf8("* ..."),
                String::utf8("* 잠깐 있어봐"),
            })
        )->set_expressions(Array::make(2, 18)));
        sys->sequence([this]() { return !global->get_player_text_box(); }, {
            [this, sans]() {
                audio_player->play("teleport");
                camera->blind(0.2, 1, 0.3);
                sans->set_position(Vector2(-420, 233));
                sans->start_walking(Vector2i(0, -1));
                sans->start_walking();
                camera->connect("finished_tween", Callable(global, "set_player_move").bind(true), CONNECT_ONE_SHOT);
            }
        });
    }
}

void LaboratoryNode::character_talk() {
}