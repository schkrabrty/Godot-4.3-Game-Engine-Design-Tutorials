#ifndef CUSTOM_FRAME_RATE_HANDLING_H
#define CUSTOM_FRAME_RATE_HANDLING_H

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/timer.hpp>
#include <godot_cpp/core/class_db.hpp>

namespace godot {

    class CustomFrameRateHandling : public Node {
        GDCLASS(CustomFrameRateHandling, Node)

    private:
        Timer *timer;
        float frame_rate;
        float dir;
        float spr_height;
        Node *parent;
        float speed;

    protected:
        static void _bind_methods();

    public:
        CustomFrameRateHandling();
        ~CustomFrameRateHandling();

        void _init();
        void _ready() override;
        void _on_timer_timeout();
        void initial_Setup();
    };
}

# endif