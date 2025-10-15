#ifndef COINSCONTROLLER_H
#define COINSCONTROLLER_H

#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/core/class_db.hpp>

namespace godot{
	class CoinsController : public Node2D {
	GDCLASS(CoinsController, Node2D)

	private:
		double distance;

	protected:
		static void _bind_methods();

	public:
		CoinsController();
		~CoinsController();

		void _init();
		void _ready() override;
		void _process(double delta) override;

		// Getter and Setter
		double get_distance() const;
		void set_distance(const double p_distance);
	};
}

#endif