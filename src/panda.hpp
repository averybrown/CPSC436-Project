#pragma once

#include "common.hpp"

class Panda : public Renderable
{
	static Texture panda_texture;

public:
	// Creates all the associated render resources and default transform
	bool init();

	// Releases all associated resources
	void destroy();

	// Update salmon position based on direction
	// ms represents the number of milliseconds elapsed from the previous update() call
	void update(float ms);

	// Renders the salmon
	void draw(const mat3& projection)override;

	// Returns the current salmon position
	vec2 get_position()const;

	// Moves the salmon's position by the specified offset
	void move(vec2 off);

	// Set salmon rotation in radians
	void set_rotation(float radians);

	// True if the salmon is alive
	bool is_alive()const;

	// Kills the salmon, changing its alive state and triggering on death events
	void kill();

	vec2 get_velocity()const;

	float get_x_velocity()const;

	float get_y_velocity()const;

	void set_velocity(vec2 vel);

	void set_x_velocity(float vel);

	void set_y_velocity(float vel);


private:
	bool m_is_alive; // True if the salmon is alive
	vec2 m_position; // Window coordinates
	vec2 m_scale; // 1.f in each dimension. 1.f is as big as the associated texture
	float m_rotation; // in radians
	size_t m_num_indices; // passed to glDrawElements
	vec2 m_velocity;
};