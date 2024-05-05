#pragma once
#include <glm/glm.hpp>


struct KeyboardEvent {
	int button = 0;
	int action = 0;
	int mods = 0;
	int scancode = 0;
	bool handled = false;
	KeyboardEvent(int key, int scancode, int action, int mods) : button(key), scancode(scancode), action(action), mods(mods) {}
};

struct ResizeEvent {
	int width = 0;
	int height = 0;
	bool handled = false;

	ResizeEvent() {};
	ResizeEvent(int w, int h) : width(w), height(h)
	{}
};

struct MouseButtonEvent {
	int button = 0;
	int action = 0;
	int mods = 0;
	bool handled = false;
	glm::vec<2, double> mousePos = { 0,0 };
};

struct MouseMoveEvent {
	glm::vec<2, double> mousePos = { 0,0 };
	glm::vec<2, double> deltaPos = { 0,0 };
	bool handled = false;

};

class MouseMoveFactory {

public:

	static void TrackMousePos(glm::vec<2, double> position)
	{
		lastPos = position;
	}

	static MouseMoveEvent CreateMouseMove(glm::vec<2, double> position)
	{
		MouseMoveEvent e = { position, position - lastPos };
		lastPos = position;
		return e;
	}

	static glm::vec<2, double> lastPos;
};