#include "main.h"
using namespace std;

#undef main
int main()
{
	// SDL 初始化
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window* pWindow = SDL_CreateWindow("Box2D's test",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
		800, 600, SDL_WINDOW_SHOWN);
	SDL_Renderer* pRenderer = 
		SDL_CreateRenderer(pWindow, -1, SDL_RENDERER_ACCELERATED);
	SDL_SetRenderDrawBlendMode(pRenderer, SDL_BLENDMODE_BLEND);
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "best");

	// 加载我们的箱子朋友
	SDL_Surface* pSurface = IMG_Load("box.png");
	SDL_Texture* pTexture = SDL_CreateTextureFromSurface(pRenderer, pSurface);

	// box2d 世界初始化
	b2Vec2 gravity(0.0f, -10.0f);
	b2World world(gravity);

	// 地面刚体
	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(40.0f, -30.0f);
	b2Body* groundBody = world.CreateBody(&groundBodyDef);

	b2PolygonShape groundBox;
	groundBox.SetAsBox(40.0f, 5.0f);

	groundBody->CreateFixture(&groundBox, 0.0f);

	// 动态刚体
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(40.0f, 10.0f);
	bodyDef.angle = (b2_pi / 4.0f);
	b2Body* body = world.CreateBody(&bodyDef);

	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(5.0f, 5.0f);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.5f;
	fixtureDef.restitution = 0.5f;
	body->CreateFixture(&fixtureDef);

	// 初始设置
	bool isQuit = false;
	float timeStep = 1.0f / 60.0f;
	int velocityIterations = 8;
	int positionIterations = 3;
	SDL_Event theEvent;
	SDL_Rect sRect = { 0, 0, 64, 64 };
	SDL_FRect dRect = { 0.0f, 0.0f, 100.0f, 100.0f };
	SDL_FPoint anchor = { 50.0f, 50.0f };

	while (!isQuit)
	{
		Uint32 theFrameStart = SDL_GetTicks();
		SDL_RenderClear(pRenderer);

		while (SDL_PollEvent(&theEvent))
		{
			switch (theEvent.type)
			{
			case SDL_QUIT:
				isQuit = true;
				break;
			default:
				break;
			}
		}

		world.Step(timeStep, velocityIterations, positionIterations);
		dRect.x = (body->GetPosition().x * 10.0f) - 50.0f;
		dRect.y = 300.0f - (body->GetPosition().y * 10.0f) - 50.0f;
		float angle = body->GetAngle() * (180.0f / b2_pi);
		SDL_RenderCopyExF(pRenderer, pTexture,
			&sRect, &dRect, angle, &anchor, SDL_FLIP_NONE);

		Uint32 theFrameEnd = SDL_GetTicks();
		if (theFrameEnd - theFrameStart < timeStep * 1000)
		{
			SDL_Delay(timeStep * 1000 - (theFrameEnd - theFrameStart));
		}
		SDL_RenderPresent(pRenderer);
	}

	SDL_DestroyTexture(pTexture); pTexture = nullptr;
	SDL_DestroyRenderer(pRenderer); pRenderer = nullptr;
	SDL_DestroyWindow(pWindow); pWindow = nullptr;
	SDL_Quit();
	return 0;
}