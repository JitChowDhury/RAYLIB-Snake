#include <iostream>
#include <raylib.h>

Color green = { 173,204,96,255 };
Color darkGreen = { 43,51,24,255 };

int cellSize = 30;
int cellCount = 25;


class Food
{
public:
	Vector2 position;
	Texture2D texture;

	Food()
	{
		Image image = LoadImage("Assets/Textures/food.png");
		texture = LoadTextureFromImage(image);
		UnloadImage(image);
		position = GenerateRandomPos();
	}

	~Food()
	{
		UnloadTexture(texture);
	}

	Vector2 GenerateRandomPos()
	{
		float x = GetRandomValue(0, cellCount - 1);
		float y = GetRandomValue(0, cellCount - 1);
		return Vector2{ x, y };
	}

	void Draw()
	{
		DrawTexture(texture, position.x * cellSize, position.y * cellSize, WHITE);
	}

};


int main() {
	//const int WINDOW_WIDTH = 750;
	//const int WINDOW_HEIGHT = 750;


	InitWindow(cellSize* cellCount, cellSize * cellCount, "RetroSnake");
	SetTargetFPS(60);

	Food food = Food();

	while (!WindowShouldClose())
	{
		BeginDrawing();//create a blank canvas
		
		//drawing
		ClearBackground(green);
		food.Draw();
		
		EndDrawing();


	}
	
	CloseWindow();
	return 0;
}