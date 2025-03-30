#include "text_trigger.h"
#include "env.h"

TextTrigger::TextTrigger() {}

TextTrigger::~TextTrigger() {}

void TextTrigger::_bind_methods() {
    ADD_SIGNAL(MethodInfo("dialogue_finished"));
    ClassDB::bind_method(D_METHOD("_on_interact_text"), &TextTrigger::_on_interact_text);

    ClassDB::bind_method(D_METHOD("set_text", "value"), &TextTrigger::set_text);
    ClassDB::bind_method(D_METHOD("get_text"), &TextTrigger::get_text);
    ADD_PROPERTY(PropertyInfo(Variant::PACKED_STRING_ARRAY, "text", PROPERTY_HINT_MULTILINE_TEXT), "set_text", "get_text");
}

void TextTrigger::_on_interact_text() {
    Ref<PackedScene> file = ResourceLoader::get_singleton()->load("res://Overworld/text_box.tscn");
    TextBox* textbox = Object::cast_to<TextBox>(file->instantiate());
    global->get_scene_container()->get_current_scene()->add_child(textbox);

    Ref<Dialogues> dia = sys->dia(); 
    dia->from(text);
    if(is_wingdings(text[0])) {
        textbox->character(TextBox::GASTER, dia);
    }else textbox->generic(dia);
    textbox->connect("dialogue_finished", Callable(this, "emit_signal").bind("dialogue_finished"));
}

bool TextTrigger::is_wingdings(String t) {
    return t.chr(0) == " ";
}

void TextTrigger::set_text(PackedStringArray value) {
    text = value;
}

PackedStringArray TextTrigger::get_text() {
    return text;
}