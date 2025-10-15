#include "MyNetworkNode.h"

using namespace godot;

MyNetworkNode::MyNetworkNode() : main_menu(nullptr), address_entry(nullptr) {
}

MyNetworkNode::~MyNetworkNode() {
}

void MyNetworkNode::_bind_methods() {
    ClassDB::bind_method(D_METHOD("_on_host_pressed"), &MyNetworkNode::_on_host_pressed);
    ClassDB::bind_method(D_METHOD("_on_join_pressed"), &MyNetworkNode::_on_join_pressed);
    ClassDB::bind_method(D_METHOD("add_player", "peer_id"), &MyNetworkNode::add_player);
    ClassDB::bind_method(D_METHOD("exit_game", "id"), &MyNetworkNode::exit_game);
    ClassDB::bind_method(D_METHOD("del_player", "id"), &MyNetworkNode::del_player);
    ClassDB::bind_method(D_METHOD("_del_player", "id"), &MyNetworkNode::_del_player);
    ClassDB::bind_method(D_METHOD("upnp_setup"), &MyNetworkNode::upnp_setup);
}

void MyNetworkNode::_ready() {
    main_menu = get_node<Control>(NodePath("CanvasLayer/MainMenu"));
    address_entry = get_node<LineEdit>(NodePath("CanvasLayer/MainMenu/MarginContainer/VBoxContainer/AddressEntry"));

    // Connect HostButton and JoinButton signals.
    Button *host_button = get_node<Button>(NodePath("CanvasLayer/MainMenu/MarginContainer/VBoxContainer/HostButton"));
    if (host_button) {
        host_button->connect("pressed", Callable(this, "_on_host_pressed"));
    } else {
        UtilityFunctions::print("HostButton not found!");
    }
    
    Button *join_button = get_node<Button>(NodePath("CanvasLayer/MainMenu/MarginContainer/VBoxContainer/JoinButton"));
    if (join_button) {
        join_button->connect("pressed", Callable(this, "_on_join_pressed"));
    } else {
        UtilityFunctions::print("JoinButton not found!");
    }

    // RPC configuration (if needed)
    Dictionary config;
    config["mode"] = MultiplayerAPI::RPCMode::RPC_MODE_ANY_PEER;
    rpc_config("_del_player", config);
}

void MyNetworkNode::_on_host_pressed() {
    peer = memnew(ENetMultiplayerPeer);
    peer->create_server(PORT);
    get_multiplayer()->set_multiplayer_peer(peer);
    get_multiplayer()->connect("peer_connected", Callable(this, "add_player"));
    add_player(get_multiplayer()->get_unique_id());
    if (main_menu) {
        main_menu->hide();
    }
    // Optionally, uncomment to set up UPnP.
    // upnp_setup();
}

void MyNetworkNode::_on_join_pressed() {
    peer = memnew(ENetMultiplayerPeer);
    if (address_entry) {
        String addr = address_entry->get_text();
        peer->create_client(addr, PORT);
    }
    get_multiplayer()->set_multiplayer_peer(peer);
    if (main_menu) {
        main_menu->hide();
    }
}

void MyNetworkNode::add_player(int peer_id) {
    // Load the scene directly from the resources.
    Ref<PackedScene> scene = ResourceLoader::get_singleton()->load("res://Scenes/Networking with CPP/network_player_cpp.tscn");
    if (!scene.is_valid()) {
        UtilityFunctions::print("Failed to load the scene!");
        return;
    }
    Node *player = scene->instantiate();
    if (player) {
        player->set_name(String::num_int64(peer_id));
        call_deferred("add_child", player);
    }
}

void MyNetworkNode::exit_game(int id) {
    get_multiplayer()->connect("peer_disconnected", Callable(this, "del_player"));
    del_player(id);
}

void MyNetworkNode::del_player(int id) {    
    rpc("_del_player", id);
}

void MyNetworkNode::_del_player(int id) {
    Node *node_to_del = get_node_or_null(NodePath(String::num_int64(id)));
    if (node_to_del) {
        node_to_del->queue_free();
    } else {
        UtilityFunctions::print("No node found for id: " + String::num_int64(id));
    }
}

void MyNetworkNode::upnp_setup() {
    UPNP *upnp = memnew(UPNP);
    int discover_result = upnp->discover();
    ERR_FAIL_COND_MSG(discover_result != UPNP::UPNP_RESULT_SUCCESS, "UPNP Discover Failed!");
    
    Ref<UPNPDevice> gateway = upnp->get_gateway();
    if (gateway.is_valid()) {
        Variant name_variant = gateway->get("friendly_name");
        UtilityFunctions::print("Gateway found: " + String(name_variant));
        bool valid = true;
        UtilityFunctions::print("Gateway valid? " + String::num_int64(valid));
    } else {
        UtilityFunctions::print("No gateway found.");
    }
    
    ERR_FAIL_COND_MSG(!gateway.is_valid(), "UPNP Invalid Gateway!");
    
    int map_result = upnp->add_port_mapping(PORT);
    ERR_FAIL_COND_MSG(map_result != UPNP::UPNP_RESULT_SUCCESS, "UPNP Port Mapping Failed!");
    
    UtilityFunctions::print("Success! Join Address: " + upnp->query_external_address());
    memdelete(upnp);
}
