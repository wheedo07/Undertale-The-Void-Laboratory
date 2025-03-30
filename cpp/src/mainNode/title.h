#ifndef TITLE_H
#define TITLE_H
#define OptionSize 3

#include<godot_cpp/classes/control.hpp>
#include<godot_cpp/classes/input_event.hpp>
#include<godot_cpp/classes/audio_stream_player.hpp>
#include<godot_cpp/variant/node_path.hpp>
namespace godot {
    class Title : public Control {
        GDCLASS(Title, Control)
        
        protected:
            static void _bind_methods();
        
        private:
            int current_pos;
            TypedArray<Node> options;

            AudioStreamPlayer* title_music;
        
        public:
            Title();
            ~Title();

            void _ready() override;
            void _input(const Ref<InputEvent>& event) override;

            void refresh_thing(int action = 0);
            int get_current_pos() const;

            void _on_blind_completed_start();
            
            void set_current_pos(int pos);
            void loop_audio();
    };
}

#endif