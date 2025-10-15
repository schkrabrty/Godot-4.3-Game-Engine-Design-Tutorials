#ifndef PARENTNODE_H
#define PARENTNODE_H

#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/core/class_db.hpp>

namespace godot {

// Forward declare KeyInputModified to avoid circular include
class KeyInputModified;

class ParentNode : public Node2D {
    GDCLASS(ParentNode, Node2D)

protected:
    static void _bind_methods();

public:
    ParentNode();
    ~ParentNode();

    void _ready() override;
    void SelectiveChildManipulator();
};

}

#endif
