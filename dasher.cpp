#include "raylib.h"

#define global_variable static

global_variable int windowHeight;
global_variable int windowWidth;

struct AnimData
{
	Rectangle rec;
	Vector2 pos;
	int frame;
	float updateTime;
	float runningTime;
};

bool isOnGround(AnimData *data)
{
	return data->pos.y >= windowHeight - data->rec.height;
}

AnimData updateAnimData(AnimData *data, float deltaTime, int maxFrame)
{
	data->runningTime += deltaTime;
	if (data->runningTime >= data->updateTime)
	{
		data->runningTime = 0.0f;
		// update animation frame
		data->rec.x = data->frame * data->rec.width;
		data->frame++;
		if (data->frame > maxFrame)
		{
			data->frame = 0;
		}
	}
	return *data;
}

int main()
{
	windowHeight = 380;
	windowWidth = 512;

	// Initialize the window
	InitWindow(windowWidth, windowHeight, "Dapper Dasher!");

	const int gravity{1'000};

	// nebula variables
	Texture2D nebula = LoadTexture("textures/12_nebula_spritesheet.png");
	const int sizeOfNebulae{5};
	AnimData nebulae[sizeOfNebulae]{};

	for (int i = 0; i < sizeOfNebulae; i++)
	{
		nebulae[i].rec.x = 0.0f;
		nebulae[i].rec.y = 0.0f;
		nebulae[i].rec.width = nebula.width/8;
		nebulae[i].rec.height = nebula.height/8;
		nebulae[i].pos.y = windowHeight - nebula.height/8;
		nebulae[i].frame = 0;
		nebulae[i].runningTime = 0.0f;
		nebulae[i].updateTime = 0.0f;

		nebulae[i].pos.x = windowWidth + i * 300;
	}

	float finishLine{ nebulae[sizeOfNebulae - 1].pos.x };

	// nebula X velocity (pixels/second)
	int nebVel{-200};

	// Player Pawn
	Texture2D playerSprite = LoadTexture("textures/scarfy.png");
	AnimData player;
	player.rec.width = playerSprite.width/6;
	player.rec.height = playerSprite.height;
	player.rec.x = 0;
	player.rec.y = 0;
	player.pos.x = windowWidth/2 - player.rec.width/2;
	player.pos.y = windowHeight - player.rec.height;
	player.frame = 0;
	player.updateTime = 1.0/12.0;
	player.runningTime = 0.0;

	// Check player
	bool isInAir{};
	// Jump velocity is pixels/second
	const int jumpVel{-600};

	int velocity{0};

	Texture2D background = LoadTexture("textures/far-buildings.png");
	float bgX{};

	Texture2D midground = LoadTexture("textures/back-buildings.png");
	float mgX{};

	Texture2D foreground = LoadTexture("textures/foreground.png");
	float fgX{};

	bool collision{};

	// Game loop starts here
	SetTargetFPS(60);
	while(!WindowShouldClose())
	{
		// Delta time (time since last frame)
		const float dT{GetFrameTime()};
		BeginDrawing();
		ClearBackground(WHITE);

		// Scroll logic here
		bgX -= 20 * dT;
		if (bgX <= -background.width*2)
		{
			bgX = 0.0f;
		}

		mgX -= 40 * dT;
		if (mgX <= -midground.width*2)
		{
			mgX = 0.0f;
		}

		fgX -= 80 *dT;
		if (fgX <= -foreground.width*2)
		{
			fgX = 0.0f;
		}

		// Draw the background
		// The two pictures are because we are displaying parts of each
		Vector2 bg1Pos{bgX, 0.0};
		DrawTextureEx(background, bg1Pos, 0.0f, 2.0f, WHITE);
		Vector2 bg2Pos{bgX + background.width*2, 0.0};
		DrawTextureEx(background, bg2Pos, 0.0f, 2.0f, WHITE);

		// Draw the midground
		Vector2 mg1Pos{mgX, 0.0};
		DrawTextureEx(midground, mg1Pos, 0.0, 2.0, WHITE);
		Vector2 mg2Pos{mgX + midground.width*2, 0.0};
		DrawTextureEx(midground, mg2Pos, 0.0, 2.0, WHITE);

		// Draw foreground
		Vector2 fg1Pos{fgX, 0.0};
		DrawTextureEx(foreground, fg1Pos, 0.0, 2.0, WHITE);
		Vector2 fg2Pos{fgX + foreground.width*2, 0.0};
		DrawTextureEx(foreground, fg2Pos, 0.0, 2.0, WHITE);

		// TODO(Phil): Extract into void functions
		if (isOnGround(&player))
		{
			velocity = 0;
			isInAir = false;
		}
		else
		{
			velocity += gravity * dT;
			isInAir = true;
		}

		if (IsKeyPressed(KEY_SPACE) && !isInAir)
		{
			velocity += jumpVel;
		}

		for (int i = 0; i < sizeOfNebulae; i++)
		{
			nebulae[i].pos.x += nebVel * dT;
		}

		finishLine += nebVel * dT;

		player.pos.y += velocity * dT;

		if (!isInAir)
		{
			player = updateAnimData(&player, dT, 5);
		}

		for (int i = 0; i < sizeOfNebulae; i++)
		{
			nebulae[i] = updateAnimData(&nebulae[i], dT, 7);
		}

		for (AnimData nebula : nebulae)
		{
			float pad{50};
			Rectangle nebRec{
				nebula.pos.x + pad,
				nebula.pos.y + pad,
				nebula.rec.width - 2*pad,
				nebula.rec.height - 2*pad,
			};
			Rectangle playerRec{
				player.pos.x,
				player.pos.y,
				player.rec.width,
				player.rec.height,
			};

			if (CheckCollisionRecs(nebRec, playerRec))
			{
				collision = true;
			}
		}

		// Handle win/lose condition
		if (collision)
		{
			// If a collision has been detected we lose the game
			DrawText("You Lose!", windowWidth/4, windowHeight/2, 40, RED);
		}
		else if (player.pos.x >= finishLine)
		{
			DrawText("You Won!", windowWidth/4, windowHeight/2, 40, RED);
		}
		else
		{
			for (int i = 0; i < sizeOfNebulae; i++)
			{
				DrawTextureRec(nebula, nebulae[i].rec, nebulae[i].pos, WHITE);
			}
			// Draw player pawn
			DrawTextureRec(playerSprite, player.rec, player.pos, WHITE);
		}
		EndDrawing();
	}

	// On Exit code
	UnloadTexture(playerSprite);
	UnloadTexture(nebula);
	UnloadTexture(background);
	UnloadTexture(midground);
	UnloadTexture(foreground);
	CloseWindow();
}