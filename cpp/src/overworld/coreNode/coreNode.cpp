#include "coreNode.h"
#include "env.h"
#include "engine/Overworld/Characters/enemy_overworld.h"

CoreNode::CoreNode() {}

CoreNode::~CoreNode() {}

void CoreNode::_bind_methods() {
    ClassDB::bind_method(D_METHOD("voidWall"), &CoreNode::voidWall);
    ClassDB::bind_method(D_METHOD("event1"), &CoreNode::event1);
    ClassDB::bind_method(D_METHOD("event2"), &CoreNode::event2);
    ClassDB::bind_method(D_METHOD("text_trigger1"), &CoreNode::text_trigger1);
    ClassDB::bind_method(D_METHOD("main2_sans"), &CoreNode::main2_sans);
    ClassDB::bind_method(D_METHOD("main2_start"), &CoreNode::main2_start);
    ClassDB::bind_method(D_METHOD("main2_selected_option", "option"), &CoreNode::main2_selected_option);
}

void CoreNode::ready() {
    String name = get_name();

    if(name == "core_9") {
        if(!global->get_flag("event1")) return;
        Node* text_trigger = get_node_internal("trigger/text_trigger");
        text_trigger->disconnect("dialogue_finished", Callable(this, "text_trigger1"));
        text_trigger->set("text", Array::make(
            String::utf8("* ( 화면은 더 이상 켜지지 않는다 )")
        ));
    }else if(name == "core_3") {
        main2 = Object::cast_to<OverworldAreaTrigger>(get_node_internal("trigger/main2_start"));
        EnemyOverworld* sans = Object::cast_to<EnemyOverworld>(get_node_internal("Sans"));
        if(global->get_flag("main1")) {
            sans->queue_free();
            main2->set_required_collider_group(StringName());
            return;
        }

        global->set_player_move(false);
        summontextbox()->generic(sys->dia()->from(
            PackedStringArray({
                String::utf8("* [color=red]..."),
            })
        )->set_speed(Array::make(0.03)));
        sys->executeTrue([this]() { return !global->get_player_text_box(); },
        [this, sans]() {
            sans->start_walking(Vector2i(0, 1));
        });
    }
}

void CoreNode::voidWall() {
    if(global->get_flag("is")) {
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
        global->save_flag("is", 1);
    }
    global->set_player_can_move(true);
}

void CoreNode::event1() {
    int is = global->get_flag("is");
    if(!is) {
        summontextbox()->generic(sys->dia()->from(
            PackedStringArray({
                String::utf8("* !?"), String::utf8("* 앞으로 갈수가 없다..?"),
                String::utf8("* 무언가 앞에 안보이는 벽이있는거 같다..?"),
                String::utf8("* 무언가 느낌이 안 좋다..")
            })
        ));
        global->save_flag("is", 1);
        global->set_player_can_move(true);
        return;
    }

    if(is == 2) {
        summontextbox()->generic(sys->dia()->from(
            PackedStringArray({
                String::utf8("* !?"), String::utf8("* 앞으로 갈수가 없다..?")
            })
        ));
        auto isFun = [this]() { return !global->get_player_text_box(); };
        sys->sequence({
            {[this]() {
                music_player->stop();
                summontextbox()->generic(sys->dia()->from(
                    PackedStringArray({
                        String::utf8("* [color=red]흠.. 역시..[/color]"),
                        String::utf8("* [color=red]음? 왜 파트너?[/color]"),
                        String::utf8("* [color=red]아 무슨일인지는 직접알아봐[/color]"),
                        String::utf8("* [color=red]일단 코어 갈림길로 가볼래? 나한테 묻지 말고[/color]")
                    })
                ));
            }, isFun},
            {[this]() {
                music_player->play();
                global->save_flag("is", 3);
            }, isFun}
        });
    }else summontextbox()->generic(sys->dia()->from(
        PackedStringArray({
            String::utf8("* 여기도 막혀있다...")
        })
    ));
    global->set_player_can_move(true);
}

void CoreNode::event2() {
    int is = global->get_flag("is");
    if(is < 2) {
        global->set_player_move(false);
        music_player->stop();
        if(global->get_flag("main1")) {
            summontextbox()->generic(sys->dia()->from(
                PackedStringArray({
                    String::utf8("* [color=red]흠.. 파트너?[/color]"),
                    String::utf8("* [color=red]일단 그 코미니언을 따라가자[/color]"),
                })
            ));
        }else {
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
        }
        sys->sequence({
            {[this]() {
                music_player->play();
                global->set_player_move(true);
                global->save_flag("is", 2);
            }, [this]() { return !global->get_player_text_box(); }}
        });
    }
}

void CoreNode::text_trigger1() {
    summontextbox()->character(Character::GASTER, sys->dia()->from(
        PackedStringArray({
            String::utf8("WHEN TIME AND SPACE BECOME ONE..."),
            String::utf8("ALL SUFFERING WILL CEASE TO EXIST...")
        })
    ));
    sys->sequence({
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
            global->save_flag("event1", true);
        }, [this]() { return !global->get_player_text_box(); }}
    });
}

void CoreNode::main2_sans() {
    if(get_node_or_null("Sans")) {
        music_player->play();
        global->set_player_move(true);
        global->set_flags("main1", true);
        main2->set_required_collider_group(StringName());
        get_node_internal("Sans")->queue_free();
    }else WARN_PRINT("SANS가 아닌 다른게 들어옴");
}

void CoreNode::main2_start() {
    TextBox* textbox = summontextbox();
    textbox->connect("selected_option", Callable(this, "main2_selected_option"));
    textbox->generic(sys->dia()->from(
        PackedStringArray({
            String::utf8("* 샌즈를 따라가겠습니까?")
        })
    ), Array::make(String::utf8("따라간다"), String::utf8("아니요")),
    Array::make(
        Variant(),
        sys->dia()->from(
            PackedStringArray({
                String::utf8("* 아직 할일이 있다")
            })
        )
    ));
}

void CoreNode::main2_selected_option(int option) {
    if(option == 0) {
        get_node_internal("trigger/main2_start")->queue_free();
        get_node_internal("static/1")->queue_free();
        player->call_deferred("start_walking", Vector2i(0, 1));
    }else if(option == 1) {
        player->start_walking(Vector2i(0, -1));
        sys->sleep([this]() {
            player->start_walking();
        }, 0.2);
    }
    global->set_player_can_move(true);
}