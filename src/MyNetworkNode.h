#ifndef MYNETWORKNODE_H
#define MYNETWORKNODE_H

#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/e_net_multiplayer_peer.hpp>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/line_edit.hpp>
#include <godot_cpp/classes/control.hpp>       // Needed for UI nodes like hide()
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/classes/upnp.hpp>
#include <godot_cpp/classes/upnp_device.hpp>
#include <godot_cpp/classes/multiplayer_api.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/button.hpp>  // For button signal connections
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/variant/node_path.hpp>

namespace godot {

class MyNetworkNode : public Node3D {
    GDCLASS(MyNetworkNode, Node3D);

private:
    static const int PORT = 9999;
    ENetMultiplayerPeer *peer = nullptr; // Use a raw pointer for ENetMultiplayerPeer
    Ref<PackedScene> player_scene;
    Control *main_menu = nullptr;
    LineEdit *address_entry = nullptr;

protected:
    static void _bind_methods();

public:
    MyNetworkNode();
    ~MyNetworkNode();

    // Called when the node is added to the scene.
    void _ready() override;

    // Called from UI buttons.
    void _on_host_pressed();
    void _on_join_pressed();

    // Multiplayer methods.
    void add_player(int peer_id);
    void exit_game(int id);
    void del_player(int id);
    void _del_player(int id); // RPC callable

    // UPnP setup.
    void upnp_setup();
};

} // namespace godot

#endif // MYNETWORKNODE_H
