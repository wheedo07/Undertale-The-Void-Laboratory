#ifndef TEXT_TRIGGER_H
#define TEXT_TRIGGER_H

#include<godot_cpp/classes/node2d.hpp>
#include<godot_cpp/classes/resource_loader.hpp>
namespace godot {
    class TextTrigger : public Node2D {
        GDCLASS(TextTrigger, Node2D);
        
        protected:
            static void _bind_methods();
        
        private:
            PackedStringArray text;

        public:
            TextTrigger();
            ~TextTrigger();

            void _on_interact_text();
            bool is_wingdings(String t);

            void set_text(PackedStringArray value);
            PackedStringArray get_text();
    };
}

#endif