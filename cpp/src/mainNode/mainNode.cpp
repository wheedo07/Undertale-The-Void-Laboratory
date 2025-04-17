#include "mainNode.h"
#include<godot_cpp/classes/timer.hpp>
#include "env.h"

MainNode::MainNode() {
    isClear = false;
}

MainNode::~MainNode() {}

void MainNode::_bind_methods() {
    ClassDB::bind_method(D_METHOD("clear_system", "is"), &MainNode::clear_system, DEFVAL(true));
}

void MainNode::_ready() {
    SceneContainer::_ready();
    if(isEditor) return;
    loader = ResourceLoader::get_singleton();
    sys = this;
}

void MainNode::_process(double delta) {
    system(delta);
}

Ref<Dialogues> MainNode::dia() {
    Ref<Dialogues> dialogue = memnew(Dialogues);
    return dialogue;
}

void MainNode::load_battle(String path, Vector2 vec) {
    scene_changer->load_battle("res://Battle/battle.tscn", loader->load(path), true, vec);
}