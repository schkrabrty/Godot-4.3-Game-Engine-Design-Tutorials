#include "LifetimeRigidBody.h"

using namespace godot;

LifetimeRigidBody::LifetimeRigidBody() {
}

LifetimeRigidBody::~LifetimeRigidBody() {
}

void LifetimeRigidBody::_bind_methods() {
    ClassDB::bind_method(D_METHOD("start_lifetime", "time"), &LifetimeRigidBody::start_lifetime);
    ClassDB::bind_method(D_METHOD("_on_Timer_timeout"), &LifetimeRigidBody::_on_Timer_timeout);
}

void LifetimeRigidBody::start_lifetime(float time) {
    // Allocate a Timer instance using memnew.
    Timer *timer = memnew(Timer);
    timer->set_wait_time(time);
    timer->set_one_shot(true);
    timer->set_autostart(true); // Automatically start the timer.
    
    // Add the timer as a child so it will be processed.
    add_child(timer);
    
    // Connect the timer's "timeout" signal to the _on_Timer_timeout() method.
    timer->connect("timeout", Callable(this, "_on_Timer_timeout"));
}

void LifetimeRigidBody::_on_Timer_timeout() {
    // When the timer times out, free this node.
    queue_free();
}
