#include "mainAttacks.h"
using namespace godot;

MainAttacks::MainAttacks() {}

MainAttacks::~MainAttacks() {}

void MainAttacks::_bind_methods() {
    ADD_SIGNAL(MethodInfo("throws",
        PropertyInfo(Variant::VECTOR2, "direction"),
        PropertyInfo(Variant::INT, "power")
    ));
}

void MainAttacks::ready() {
    ResourceLoader* loader = ResourceLoader::get_singleton();
    bone = loader->load("res://Battle/Bullets/Bone/bone.tscn");
    bone_spike = loader->load("res://Battle/Bullets/BoneSpike/bone_spike.tscn");
    blaster = loader->load("res://Battle/Bullets/Blaster/blaster.tscn");
}

void MainAttacks::start_attack() {
    if(part == PartType::sans_1)
        sans_1();
}

void MainAttacks::set_part(PartType type) {
    part = type;
}

Bone* MainAttacks::create_bone(Masking type) {
    Bone* boneObject = Object::cast_to<Bone>(bone->instantiate());
    add_bullet(boneObject, type);
    return boneObject;
}

BoneSpike* MainAttacks::create_boneSpike(Masking type) {
    BoneSpike* spikeObject = Object::cast_to<BoneSpike>(bone_spike->instantiate());
    add_bullet(spikeObject, type);
    return spikeObject;
}

Blaster* MainAttacks::create_blaster(Masking type) {
    Blaster* blasterObject = Object::cast_to<Blaster>(blaster->instantiate());
    add_bullet(blasterObject, type);
    return blasterObject;
}