#include "register_types.h"
#include<gdextension_interface.h>
#include<godot_cpp/core/defs.hpp>
#include<godot_cpp/godot.hpp>

void register_init() {
    GDREGISTER_CLASS(MainNode);
    GDREGISTER_CLASS(Title);
    GDREGISTER_CLASS(DropText);
    GDREGISTER_CLASS(TextTrigger);
    GDREGISTER_CLASS(MoveTrigger);

    // overworld Node
    GDREGISTER_CLASS(CoreNode);
    GDREGISTER_CLASS(LaboratoryNode);
    GDREGISTER_CLASS(TrueLaboratory);

    // enemy Node
    GDREGISTER_CLASS(Enemy_SANS1);

    // attackNode
    GDREGISTER_CLASS(AttackSans1);
}