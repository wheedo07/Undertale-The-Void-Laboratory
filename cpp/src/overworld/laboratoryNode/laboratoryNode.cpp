#include "laboratoryNode.h"
#include "env.h"
#include "engine/Overworld/Characters/enemy_overworld.h"

LaboratoryNode::LaboratoryNode() {}

LaboratoryNode::~LaboratoryNode() {}

void LaboratoryNode::_bind_methods() {
    ClassDB::bind_method(D_METHOD("init"), &LaboratoryNode::init);
}

void LaboratoryNode::init() {
    if(isEditor) return;
    camera = global->get_scene_container()->get_camera();
    initEvent();
}

void LaboratoryNode::initEvent() {
    EnemyOverworld* sans = Object::cast_to<EnemyOverworld>(get_node_internal("sans"));
    PlayerOverworld* player = Object::cast_to<PlayerOverworld>(get_node_internal("player"));
    sans->start_walking(Vector2i(-1, 0));
    player->start_walking(Vector2i(-1, 0));
    camera->blind(0.5, 0.6);
    sys->sleep([this, sans, player]() {
        sans->start_walking(Vector2i(0, 0));
        player->start_walking(Vector2i(0, 0));
        summontextbox()->generic(sys->dia()->from(
            Array::make(String::utf8("* (연구소는 평소와 다르게 음산하다...)"))
        ));
        sys->sequence([this]() { return !global->get_player_text_box(); },
        {[this]() {
            summontextbox()->character(Character::SANS, sys->dia()->from(
                Array::make(String::utf8("* 여기까지 왔으니 이제 돌아가."))
            )->set_expressions(Array::make(4)));
        }, [this, sans]() {
            sans->start_walking(Vector2(0, -1));
            sans->start_walking();
            summontextbox()->character(Character::SANS, sys->dia()->from(
                Array::make(
                    String::utf8("* 연구소 정문으로 나갈수 있어 이만 나가 줘"),
                    String::utf8("* 넌 이미 충분히 많은 걸 해왔잖아?"),
                    String::utf8("* 다른 타임라인에서 네가 한 일들..."),
                    String::utf8("* 얼마나 많은 괴물들을 죽였지?"),
                    String::utf8("* 얼마나 많은 타임라인을 망쳐놨지?"),
                    String::utf8("* 나는 기억하고 있어 너가 수많은 리셋으로 이르킨 일들..")
                )
            )->set_expressions(Array::make(4, 19, 20, 5, 15, 17))->
            set_speed(Array::make(0.08, 0.08, 0.08, 0.08, 0.08, 0.11)));
        }, [this]() {
            global->set_player_text_box(true);
            audio_player->play("beep");
            sys->sleep([this]() {
                camera->Void(0.5, 0.5, 0.002);
                summontextbox()->character(Character::SANS, sys->dia()->from(
                    Array::make(
                        String::utf8("* ...제길")
                    )
                )->set_expressions(Array::make(18))->set_speed(Array::make(0.2)));
            }, 3);
        }, [this]() {
            global->set_player_text_box(true);
            sys->sleep([this]() {
                summontextbox()->character(Character::SANS, sys->dia()->from(
                    Array::make(
                        String::utf8("* ...하지만 지금은."),
                        String::utf8("* 더 큰 문제가 생긴거 같네..")
                    )
                )->set_expressions(Array::make(4, 21)));
            }, 2);
        }, [this]() {
        }});
    }, 1.5);
}