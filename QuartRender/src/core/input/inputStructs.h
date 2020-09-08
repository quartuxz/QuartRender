#pragma once


struct KeyboardInput {
	int key;
	int scancode;
	int action;
	int mods;
	bool captureByIMGUI;
	bool isValid;

};


struct CursorPosition {
	double xpos;
	double ypos;
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