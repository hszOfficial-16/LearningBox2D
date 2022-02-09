#include "scene.h"

#include <iostream>

class TestScene : public GameScene
{
public:
	TestScene()
	{
		GameImage* box = new GameImage("resource\\box.png");
		GameImage* groundImage = new GameImage("resource\\ground.png");

		Character* ground = nullptr;
		{
			b2BodyDef bodyDef;
			bodyDef.position = { 0.0f, -3.0f };
			ground = new Character(GetWorld(), &bodyDef);

			b2PolygonShape shape;
			shape.SetAsBox(7.0f, 0.5f);
			ground->GetBody()->CreateFixture(&shape, 0.0f);
			ground->AddImage(groundImage);
			AddCharacter(ground);

			b2EdgeShape bound;
			bound.SetTwoSided({ -10.0f, -10.0f }, { 10.0f, -10.0f });
			ground->GetBody()->CreateFixture(&bound, 0.0f);
			bound.SetTwoSided({ -10.0f, 10.0f }, { 10.0f, 10.0f });
			ground->GetBody()->CreateFixture(&bound, 0.0f);
			bound.SetTwoSided({ -10.0f, -10.0f }, { -10.0f, 10.0f });
			ground->GetBody()->CreateFixture(&bound, 0.0f);
			bound.SetTwoSided({ 10.0f, -10.0f }, { 10.0f, 10.0f });
			ground->GetBody()->CreateFixture(&bound, 0.0f);
		}

		Character* myBox[5];

		for (int i = 0; i < 5; i++)
		{
			b2BodyDef bodyDef;
			bodyDef.type = b2_dynamicBody;
			bodyDef.angle = b2_pi / (6 - i);
			bodyDef.position = { -5.0f + 2.0f * i, 3.0f };
			myBox[i] = new Character(GetWorld(), &bodyDef);

			b2PolygonShape shape;
			shape.SetAsBox(0.5f, 0.5f);
			b2FixtureDef fixtureDef;
			fixtureDef.density = 1.0f;
			fixtureDef.restitution = 1.1f;
			fixtureDef.shape = &shape;
			myBox[i]->GetBody()->CreateFixture(&fixtureDef);
			myBox[i]->AddImage(box);
			AddCharacter(myBox[i]);
		}

		Listener* listenWheel = nullptr;
		{
			listenWheel = new Listener(SDL_MOUSEWHEEL, [&](SDL_Event* pEvent) {
				if (pEvent->wheel.y > 0)
				{
					SetCameraZoomSpeed(1.1f);
				}
				else if (pEvent->wheel.y < 0)
				{
					SetCameraZoomSpeed(0.9f);
				}
				});
			ListenerManager::GetInstance().Register(listenWheel);
		}

		Listener* listenKeyboard = nullptr;
		{
			listenKeyboard = new Listener(SDL_KEYDOWN, [=](SDL_Event* pEvent) {
				switch (pEvent->key.keysym.sym)
				{
				case SDLK_1:
					CameraFollow(myBox[0]->GetBody());
					break;
				case SDLK_2:
					CameraFollow(myBox[1]->GetBody());
					break;
				case SDLK_3:
					CameraFollow(myBox[2]->GetBody());
					break;
				case SDLK_4:
					CameraFollow(myBox[3]->GetBody());
					break;
				case SDLK_5:
					CameraFollow(myBox[4]->GetBody());
					break;
				case SDLK_0:
					CameraFollow(nullptr);
				}
				});
			ListenerManager::GetInstance().Register(listenKeyboard);
		}
	}

	void Step() override
	{
		GameScene::Step();
	}

	static GameScene* Create()
	{
		return new TestScene;
	}
};

static int sceneIndex = SceneManager::GetInstance().RegisterScene("test", TestScene::Create);