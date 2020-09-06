#pragma once


struct KeyboardInput {
	int key;
	int scancode;
	int action;
	int mods;
	bool captureByIMGUI;
	bool isValid;

};