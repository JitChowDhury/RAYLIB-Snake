#include <iostream>
#include <raylib.h>

Color green = { 173,204,96,255 };
Color darkGreen = { 43,51,24,255 };

int main() {
	const int WINDOW_WIDTH = 750;
	const int WINDOW_HEIGHT = 750;


	InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "RetroSnake");
	SetTargetFPS(60);

	while (!WindowShouldClose())
	{
		BeginDrawing();//create a blank canvas
		
		//drawing
		ClearBackground(green);
		
		EndDrawing();


	}
	
	CloseWindow();
	return 0;
}