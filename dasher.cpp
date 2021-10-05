#include "raylib.h"

#define global_variable static

global_variable int windowHeight;

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
	// Array with windows info
	int windowDimensions[2];
	windowDimensions[0] = 512;
	windowDimensions[1] = 380;

	// Initialize the window
	InitWindow(windowDimensions[0], windowDimensions[1], "Dapper Dasher!");

	const int gravity{1'000};

	// nebula variables
	Texture2D nebula = LoadTexture("textures/12_nebula_sprintsheet.png");
	const int sizeofNebulae{3};
	AnimData nebulae[sizeofNebulae]{};

	for (int i = 0; i < sizeofNebulae; i++)
	{
		nebulae[i].rec.x = 0.0f;
		nebulae[i].rec.y = 0.0f;
		nebulae[i].rec.width = nebula.width/8;
		nebulae[i].rec.height = nebula.height/8;
		nebulae[i].pos.y = windowDimensions[1] - nebula.height/8;
		nebulae[i].frame = 0;
		nebulae[i].runningTime = 0.0f;
		nebulae[i].updateTime = 0.0f;

		nebulae[i].pos.x = windowDimensions[0] + i * 300;
	}

	float finishLine{ nebulae[sizeofNebulae - 1].pos.x };

	// nebula X velocity (pixels/second)
	int nebVel{-100};

	// Player Pawn
	Texture2D playerSprite = LoadTexture("textures/scarfy.png");
	AnimData player;
	player.rec.width = playerSprite.width/6;
	player.rec.height = playerSprite.height;
	player.rec.x = 0;
	player.rec.y = 0;
	player.pos.x = windowDimensions[0]/2 - player.rec.width/2;
	player.pos.y = windowDimensions[1] - player.rec.height;
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

		// Draw nebula
		DrawTextureRec(nebula, nebulae[0].rec, nebulae[0].pos, WHITE);
		DrawTextureRec(playerSprite, player.rec, player.pos, WHITE);

		EndDrawing();
	}

	// On Exit code
	UnloadTexture(nebula);
	CloseWindow();
}