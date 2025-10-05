#include <iostream>
#include <raylib.h>
#include <deque>
#include <raymath.h>

Color green = { 173,204,96,255 };
Color darkGreen = { 43,51,24,255 };
Color background = { 240, 230, 200, 255 };
Color snakeColor = { 40, 20, 50, 255 };


int cellSize = 30;
int cellCount = 25;
int offset = 75;


double lastUpdateTime = 0;

bool ElementInDeque(Vector2 element, std::deque<Vector2> deque)
{
	for (unsigned int i = 0; i < deque.size(); i++)
	{
		if (Vector2Equals(deque[i], element))
		{
			return true;
		}
	}
	return false;
}


bool eventTriggered(double interval)
{
	double currentTime = GetTime();
	if (currentTime - lastUpdateTime >= interval)
	{
		lastUpdateTime = currentTime;
		return true;
	}
	return false;
}

class Snake
{
public:
	std::deque<Vector2> body = { Vector2{6,9},Vector2{5,9},Vector2{4,9} };
	Vector2 direction = { 1,0 };
	bool addSegment = false;

	void Draw()
	{
		for (int i = 0; i < body.size(); i++)
		{
			float  x = body[i].x;
			float  y = body[i].y;
			Rectangle segment = Rectangle{offset+ x * cellSize,offset+ y * cellSize, (float)cellSize, (float)cellSize };

			DrawRectangleRounded(segment,0.5,6 , snakeColor);
		}
	}


	void Update()
	{
		body.push_front(Vector2Add(body[0], direction));

		if (addSegment == true)
		{
			addSegment = false;
		}
		else
		{
		body.pop_back();
		}
	}

	void Reset()
	{
		body= { Vector2{6,9},Vector2{5,9},Vector2{4,9} };
		direction = { 1,0 };

	}
};



class Food
{
public:
	Vector2 position;
	Texture2D texture;

	Food(std::deque<Vector2> snakeBody)
	{
		Image image = LoadImage("Assets/Textures/foodApple.png");
		texture = LoadTextureFromImage(image);
		UnloadImage(image);
		position = GenerateRandomPos(snakeBody);
	}

	~Food()
	{
		UnloadTexture(texture);
	}

	Vector2 GenerateRandomCell()
	{
		float x = (float)GetRandomValue(0, cellCount - 1);
		float y = (float)GetRandomValue(0, cellCount - 1);
		return Vector2{ x,y };
	}

	Vector2 GenerateRandomPos(std::deque<Vector2> snakeBody)
	{

		 position = GenerateRandomCell();
		while (ElementInDeque(position, snakeBody))
		{
			position = GenerateRandomCell();
		}
		return position;
	}

	void Draw()
	{
		DrawTexture(texture, offset + (int)position.x * cellSize, offset + (int)position.y * cellSize, WHITE);
	}

};


class Game
{
public:
	Snake snake = Snake();
	Food food = Food(snake.body);
	bool running = true;
	int score = 0;

	Sound eatSound;
	Sound wallSound;
	Music bgMusic;

	Game()
	{
		InitAudioDevice();
		SetMasterVolume(1.0f);
		eatSound = LoadSound("Assets/Sounds/eat.wav");
		wallSound = LoadSound("Assets/Sounds/wall.wav");

		bgMusic = LoadMusicStream("Assets/Sounds/bgm.wav");
		PlayMusicStream(bgMusic);
		
		SetMusicVolume(bgMusic, 0.05f); 

		SetSoundVolume(eatSound, 1.0f);
		SetSoundVolume(wallSound, 1.0f);
	}

	~Game()
	{
		UnloadSound(eatSound);
		UnloadSound(wallSound);
		UnloadMusicStream(bgMusic);
		CloseAudioDevice();

	}

	void Draw()
	{
		food.Draw();
		snake.Draw();
	}

	void Update()
	{

		if (running)
		{
			snake.Update();
			CheckCollisionWithFood();
			CheckCollisionWithEdges();
			CheckCollisionWithTail();
		}

	}

	void CheckCollisionWithFood()
	{
		if (Vector2Equals(snake.body[0], food.position))
		{
			food.position = food.GenerateRandomPos(snake.body);
			snake.addSegment = true;
			PlaySound(eatSound);
			score++;
		}
	}

	void CheckCollisionWithEdges()
	{
		if (snake.body[0].x == cellCount || snake.body[0].x == -1)
		{
			GameOver();
		}
		if (snake.body[0].y == cellCount || snake.body[0].y == -1)
		{
			GameOver();
		}
	}

	void GameOver()
	{
		snake.Reset();
		food.position = food.GenerateRandomPos(snake.body);
		PlaySound(wallSound);
		running = false;
		score = 0;
	}

	void CheckCollisionWithTail()
	{
		std::deque<Vector2> headlessBody = snake.body;
		headlessBody.pop_front();
		if (ElementInDeque(snake.body[0], headlessBody))
		{
			GameOver();
		}
	}
};

int main() {
	//const int WINDOW_WIDTH = 750;
	//const int WINDOW_HEIGHT = 750;


	InitWindow(2 * offset + cellSize* cellCount, 2 * offset + cellSize * cellCount, "RetroSnake");
	SetTargetFPS(60);
	Game game = Game();

	while (!WindowShouldClose())
	{
		UpdateMusicStream(game.bgMusic);
		if (GetMusicTimePlayed(game.bgMusic) >= GetMusicTimeLength(game.bgMusic)) {
			StopMusicStream(game.bgMusic);
			PlayMusicStream(game.bgMusic);
		}
		BeginDrawing();//create a blank canvas

		if (eventTriggered(0.2))
		{
			game.Update();
		}
		if (IsKeyPressed(KEY_W) && game.snake.direction.y != 1)
		{
			game.snake.direction = { 0,-1 };
			game.running = true;

		}
		else if (IsKeyPressed(KEY_S) && game.snake.direction.y != -1)
		{
			game.snake.direction = { 0,1 };
			game.running = true;
		}
		else if (IsKeyPressed(KEY_A) && game.snake.direction.x != 1)
		{
			game.snake.direction = { -1,0 };
			game.running = true;
		}
		else if (IsKeyPressed(KEY_D) && game.snake.direction.x != -1)
		{
			game.snake.direction = { 1,0 };
			game.running = true;
		}


		//drawing
		ClearBackground(background);
		DrawRectangleLinesEx(Rectangle{(float)offset - 5, (float)offset - 5, (float)cellSize * cellCount + 10, (float)cellSize *cellCount + 10
	}, 5, darkGreen);

		DrawText("Retro Snake", offset - 5, 20, 40, BLACK);
		DrawText(TextFormat("Score: %i", game.score), offset - 5, offset + cellSize * cellCount + 10, 40, BLACK);
		game.Draw();
		
		EndDrawing();
		


	}

	CloseWindow();
	return 0;
}