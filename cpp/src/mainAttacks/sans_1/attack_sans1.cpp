#include "attack_sans1.h"
using namespace godot;

AttackSans1::AttackSans1() {}

AttackSans1::~AttackSans1() {}

void AttackSans1::_bind_methods() {
    ADD_SIGNAL(MethodInfo("throws",
        PropertyInfo(Variant::VECTOR2, "direction"),
        PropertyInfo(Variant::INT, "power")
    ));
}

void AttackSans1::ready() {
    ResourceLoader* loader = ResourceLoader::get_singleton();
    bone = loader->load("res://Battle/Bullets/Bone/bone.tscn");
    bone_spike = loader->load("res://Battle/Bullets/BoneSpike/bone_spike.tscn");
    blaster = loader->load("res://Battle/Bullets/Blaster/blaster.tscn");
}

void AttackSans1::start_attack() {
    if(main->turn_number == 0)
        turn_0();
}

Bone* AttackSans1::create_bone(Masking type) {
    Bone* boneObject = Object::cast_to<Bone>(bone->instantiate());
    add_bullet(boneObject, type);
    return boneObject;
}

BoneSpike* AttackSans1::create_boneSpike(Masking type) {
    BoneSpike* spikeObject = Object::cast_to<BoneSpike>(bone_spike->instantiate());
    add_bullet(spikeObject, type);
    return spikeObject;
}

Blaster* AttackSans1::create_blaster(Masking type) {
    Blaster* blasterObject = Object::cast_to<Blaster>(blaster->instantiate());
    add_bullet(blasterObject, type);
    return blasterObject;
}