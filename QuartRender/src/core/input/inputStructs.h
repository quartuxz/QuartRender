#pragma once

//todo: add flags for custom own GUI for when IT captures the mouse and keyboard(as opposed to imgui)
struct KeyboardInput {
	int key;
	int scancode;
	int action;
	int mods;
	bool capturedByIMGUI;
	bool isValid;

};


struct CursorPosition {
	double xpos;
	double ypos;
	bool capturedByIMGUI;
};


struct MouseButtonInput {
	int button;
	int action;
	int mods;
	CursorPosition cursorPosition;
	bool isValid;
};

struct ScrollInput {
	double xoffset;
	double yoffset;
	CursorPosition cursorPosition;
	bool isValid;
};