#include "coreNode.h"
#include "env.h"
#include "engine/Overworld/Characters/enemy_overworld.h"
#include<godot_cpp/classes/reference_rect.hpp>

CoreNode::CoreNode() {}

CoreNode::~CoreNode() {}

void CoreNode::_bind_methods() {
    ClassDB::bind_method(D_METHOD("voidWall"), &CoreNode::voidWall);
    ClassDB::bind_method(D_METHOD("event1"), &CoreNode::event1);
    ClassDB::bind_method(D_METHOD("event2"), &CoreNode::event2);
    ClassDB::bind_method(D_METHOD("text_trigger1"), &CoreNode::text_trigger1);
}

void CoreNode::ready() {
    camera = global->get_scene_container()->get_camera();
    init();
}

void CoreNode::init() {
    String name = get_name();
    Dictionary flags = global->get_flags();
    if(flags["event1"] || flags["main1"])
        camera->Void(0.5, 5, 0.002);
    else camera->Void(0.5);

    if(name == "core_9") {
        int event1 = flags["event1"];
        if(!event1) return;
        Node* text_trigger = get_node_internal("trigger/text_trigger");
        text_trigger->disconnect("dialogue_finished", Callable(this, "text_trigger1"));
        text_trigger->set("text", Array::make(
            String::utf8("* ( 화면은 더 이상 켜지지 않는다 )")
        ));
    }else if(name == "core_3") {
        int main1 = flags["main1"];
        RoomEntranceNode* room_lab = Object::cast_to<RoomEntranceNode>(get_node_internal("trigger/room_lab"));
        if(main1) {
            room_lab->set_position(Vector2(281, 351));
            return;
        }

        global->set_player_move(false);
        EnemyOverworld* sans = Object::cast_to<EnemyOverworld>(get_node_internal("Sans"));
        summontextbox()->generic(sys->dia()->from(
            PackedStringArray({
                String::utf8("* ...."),
                String::utf8("* ( 당신은 코어 구조부터 모든게 이상하다는것을 느낀다.. )")
            })
        ));
        sys->sequence([this]() { return !global->get_player_text_box(); },
        {[this, sans]() {
            global->set_player_text_box(true);
            sans->set_position(Vector2(321, 356));
            sans->start_walking(Vector2i(0, -1));
            sys->sleep([this]() { global->set_player_text_box(false); }, 1.3);
        }, [this, sans]() {
            global->set_player_text_box(true);
            Vector2 player_pos = global->get_player_position();
            Vector2 sans_pos = sans->get_position();
            Vector2 diff = player_pos - sans_pos;
            Vector2i direction;
            
            if (abs(diff.x) > abs(diff.y))
                direction = Vector2i(diff.x > 0 ? 1 : -1, 0);
            else direction = Vector2i(0, diff.y > 0 ? 1 : -1);
            
            sans->start_walking(direction);
            sans->start_walking();
            sys->sleep([this]() { global->set_player_text_box(false); }, 1);
        }, [this]() {
            summontextbox()->character(Character::SANS, sys->dia()->from(
                PackedStringArray({
                    String::utf8("* 음 kid 여기 있었네.."),
                    String::utf8("* ..."),
                    String::utf8("* 뼈밖에서 보니까 반갑네"),
                    String::utf8("* 꽤 바빠보이던데... Love 15라..."),
                    String::utf8("* 그나저나 이상하지 않아? 기계는 작동을 안하고..."),
                    String::utf8("* 아. 근데 말이야, kid..."),
                    String::utf8("* 내가 여기 있는것도 우연은 아니고..."),
                    String::utf8("* 지금 당장 이걸 끝내야 해."),
                    String::utf8("* 그게 나을거야, 꼬맹아")
                })
            )->set_expressions(Array::make(2, 4, 3, 1, 0, 4, 4, 18, 19))->
            set_speed(Array::make(0.1, 0.1)));
        }, [this]() {
            sys->load_battle("res://Game/encounters/sans_1.tres", Vector2(324, 323));
        }});
    }
}

void CoreNode::voidWall() {
    int is = global->get_flags()["is"];
    if(is) {
        summontextbox()->generic(sys->dia()->from(
            PackedStringArray({
                String::utf8("* 안보이는 벽이 있는거 같다.."),
                String::utf8("* ( 다른 길을 찾아야 할거 같다.. )")
            })
        ));
    }else {
        summontextbox()->generic(sys->dia()->from(
            PackedStringArray({
                String::utf8("* !?"), String::utf8("* 앞으로 갈수가 없다..?"),
                String::utf8("* 무언가 앞에 안보이는 벽이있는거 같다..?"),
                String::utf8("* ( 다른 길을 찾아야 할거 같다... )")
            })
        ));
        global->set_flag("is", 1);
    }
    global->set_player_can_move(true);
}

void CoreNode::event1() {
    int is = global->get_flags()["is"];
    if(!is) {
        summontextbox()->generic(sys->dia()->from(
            PackedStringArray({
                String::utf8("* !?"), String::utf8("* 앞으로 갈수가 없다..?"),
                String::utf8("* 무언가 앞에 안보이는 벽이있는거 같다..?"),
                String::utf8("* 무언가 느낌이 안 좋다..")
            })
        ));
        global->set_flag("is", 1);
        global->set_player_can_move(true);
        return;
    }

    if(is == 2) {
        summontextbox()->generic(sys->dia()->from(
            PackedStringArray({
                String::utf8("* !?"), String::utf8("* 앞으로 갈수가 없다..?")
            })
        ));
        sys->sequence([this]() { return !global->get_player_text_box(); },
        {[this]() {
            Music->stop();
            summontextbox()->generic(sys->dia()->from(
                PackedStringArray({
                    String::utf8("* [color=red]흠.. 역시..[/color]"),
                    String::utf8("* [color=red]음? 왜 파트너?[/color]"),
                    String::utf8("* [color=red]아 무슨일인지는 직접알아봐[/color]"),
                    String::utf8("* [color=red]일단 코어 갈림길로 가볼래? 나한테 묻지 말고[/color]")
                })
            ));
        },
        [this]() {
            Music->play();
            global->set_flag("is", 3);
        }});
    }else summontextbox()->generic(sys->dia()->from(
        PackedStringArray({
            String::utf8("* 여기도 막혀있다...")
        })
    ));
    global->set_player_can_move(true);
}

void CoreNode::event2() {
    int is = global->get_flags()["is"];
    if(is < 2) {
        global->set_player_move(false);
        Music->stop();
        PackedStringArray text = PackedStringArray({
            String::utf8("* [color=red]흠.. 뭔가 이상하네..[/color]"),
            String::utf8("* [color=red]그렇지 않아? 파트너? 내가 벌써 나오고..[/color]"),
            String::utf8("* [color=red]( 너는 아직도 영혼을 안줬고... :) )[/color]")
        });
        if(is == 1) {
            text.append_array(PackedStringArray({
                String::utf8("* [color=red]주변은 또 막혀있고 말이야..[/color]"),
                String::utf8("* [color=red]일단 코어 갈림길로 가볼래? 거기에 있는거 같거든[/color]")
            }));
            summontextbox()->generic(
                sys->dia()->from(text)->set_speed(Array::make(0.3, 0.18, 0.18, 0.2, 0.1))
            );
        }else {
            text.append(String::utf8("* [color=red]일단 뭐 엘레베이터로 돌아가볼래? 뭔가 느낌이 안좋거든[/color]"));
            summontextbox()->generic(
                sys->dia()->from(text)->set_speed(Array::make(0.3, 0.18, 0.18, 0.1))
            );
        }
        sys->sequence([this]() { return !global->get_player_text_box(); },
        {[this]() {
            Music->play();
            global->set_player_move(true);
            global->set_flag("is", 2);
        }});
    }
}

void CoreNode::text_trigger1() {
    summontextbox()->character(Character::GASTER, sys->dia()->from(
        PackedStringArray({
            String::utf8("WHEN TIME AND SPACE BECOME ONE..."),
            String::utf8("ALL SUFFERING WILL CEASE TO EXIST...")
        })
    ));
    sys->sequence([this]() { return !global->get_player_text_box(); },
    {[this]() {
        summontextbox()->generic(sys->dia()->from(
            PackedStringArray({
                String::utf8("* ?"),
                String::utf8("* 이해 할수 없는 문양이다..")
            })
        ));

        Node* text_trigger = get_node_internal("trigger/text_trigger");
        text_trigger->disconnect("dialogue_finished", Callable(this, "text_trigger1"));
        text_trigger->set("text", PackedStringArray({
            String::utf8("* ( 아까 문자는 뭐였을까..? )")
        }));
        global->set_flag("event1", true);
    }});
}