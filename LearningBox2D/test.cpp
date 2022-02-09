#include "scene.h"

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
			shape.SetAsBox(6.0f, 0.5f);
			ground->GetBody()->CreateFixture(&shape, 0.0f);
			ground->AddImage(groundImage);
			AddCharacter(ground);
		}

		Character* myBox[5];
		for (int i = 0; i < 5; i++)
		{
			b2BodyDef bodyDef;
			bodyDef.angle = b2_pi / (i + 1);
			bodyDef.type = b2_dynamicBody;
			bodyDef.position = { -5.0f + 2.0f * i, 3.0f };
			myBox[i] = new Character(GetWorld(), &bodyDef);

			b2PolygonShape shape;
			shape.SetAsBox(0.5f, 0.5f);
			b2FixtureDef fixtureDef;
			fixtureDef.density = 1.0f;
			fixtureDef.restitution = 0.8f;
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

		CameraFollow(myBox[0]->GetBody());
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