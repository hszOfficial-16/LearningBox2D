#include "scene.h"

#include <iostream>

class TestScene : public GameScene
{
public:
	TestScene()
	{
		GameImage* box = new GameImage("resource\\box.png");

		Character* ground = nullptr;
		{
			b2BodyDef bodyDef;
			bodyDef.position = { 0.0f, -3.0f };
			ground = new Character(GetWorld(), &bodyDef);

			b2EdgeShape edgeShape;
			edgeShape.SetTwoSided({ -20.0f, 0.0f }, { 20.0f, 0.0f });
			ground->GetBody()->CreateFixture(&edgeShape, 0.0f);
			ground->AddImage(box);
			AddCharacter(ground);
		}

		Character* myBox = nullptr;
		{
			b2BodyDef bodyDef;
			bodyDef.type = b2_dynamicBody;
			bodyDef.position = { 0.0f, 3.0f };
			myBox = new Character(GetWorld(), &bodyDef);

			b2PolygonShape shape;
			shape.SetAsBox(0.5f, 0.5f);
			b2FixtureDef fixtureDef;
			fixtureDef.density = 1.0f;
			fixtureDef.restitution = 1.0f;
			fixtureDef.shape = &shape;
			myBox->GetBody()->CreateFixture(&fixtureDef);
			myBox->AddImage(box);
			AddCharacter(myBox);
		}

		CameraFollow(myBox->GetBody());
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