#ifndef TrueLaboratory_H
#define TrueLaboratory_H
#include "engine/Overworld/overworld.h"
namespace godot {
    class TrueLaboratory : public Overworld {
        GDCLASS(TrueLaboratory, Overworld);

        protected:
            static void _bind_methods();

        private:
            void flowey_move();

        public:
            TrueLaboratory();
            ~TrueLaboratory();

            void ready() override;
    };
}

#endif