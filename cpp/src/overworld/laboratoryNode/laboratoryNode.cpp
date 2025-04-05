#include "laboratoryNode.h"
#include "env.h"
#include "engine/Overworld/Characters/enemy_overworld.h"

LaboratoryNode::LaboratoryNode() {
    is = false;
}

LaboratoryNode::~LaboratoryNode() {}

void LaboratoryNode::_bind_methods() {
    ClassDB::bind_method(D_METHOD("init"), &LaboratoryNode::init);
}

void LaboratoryNode::ready() {
    camera->blind(0.3);
    camera->connect("finished_tween", Callable(this, "init"), CONNECT_ONE_SHOT);
}

void LaboratoryNode::init() {
    Dictionary flags = global->get_flags();
    EnemyOverworld* sans = Object::cast_to<EnemyOverworld>(get_node_internal("Sans"));

    if(!flags["main2"]) {
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
                        String::utf8("* ..?")
                    })
                )->set_speed(Array::make(0.03))->set_expressions(Array::make(10)));
            }
        });
    }
}