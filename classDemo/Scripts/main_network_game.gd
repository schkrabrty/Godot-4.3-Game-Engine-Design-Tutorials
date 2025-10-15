extends Node3D

const PORT = 9999
var peer = ENetMultiplayerPeer.new()
@export var player_scene : PackedScene
@onready var address_entry = $CanvasLayer/MainMenu/MarginContainer/VBoxContainer/AddressEntry
@onready var main_menu = $CanvasLayer/MainMenu

func _on_host_pressed() -> void:
	peer.create_server(PORT)
	multiplayer.multiplayer_peer = peer
	multiplayer.peer_connected.connect(add_player)
	#add_player()
	add_player(multiplayer.get_unique_id())
	main_menu.hide()
	#upnp_setup()


func _on_join_pressed() -> void:
	#peer.create_client("localhost", PORT)
	peer.create_client(address_entry.text, PORT)
	multiplayer.multiplayer_peer = peer
	main_menu.hide()


#func add_player(id = 1):
func add_player(peer_id):
	var player = player_scene.instantiate()
	#player.name = str(id)
	player.name = str(peer_id)
	call_deferred("add_child", player)


func exit_game(id):
	multiplayer.peer_disconnected.connect(del_player)
	del_player(id)


func del_player(id):
	rpc("_del_player", id)


@rpc("any_peer", "call_local")
func _del_player(id):
	get_node(str(id)).queue_free()


func upnp_setup():
	var upnp = UPNP.new()
	
	var discover_result = upnp.discover()
	assert(discover_result == UPNP.UPNP_RESULT_SUCCESS, "UPNP Discover Failed! Error %s" % discover_result)
	
	var gateway = upnp.get_gateway()
	if gateway:
		print("Gateway found: ", gateway)
		print("Gateway valid? ", gateway.is_valid_gateway())
	else:
		print("No gateway found.")
	
	assert(gateway and gateway.is_valid_gateway(), "UPNP Invalid Gateway!")
	
	var map_result = upnp.add_port_mapping(PORT)
	assert(map_result == UPNP.UPNP_RESULT_SUCCESS, "UPNP Port Mapping Failed! Error %s" % map_result)
	
	print("Success! Join Address: %s" % upnp.query_external_address())
	
