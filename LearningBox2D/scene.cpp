#include "scene.h"

// GameScene

void GameScene::Step()
{
	// Box2D 世界更新
	m_pWorld->Step(1.0f / g_settings.m_fHertz, g_settings.m_nVelocityIterations, g_settings.m_nPositionIterations);

	// 更新所有与刚体绑定的角色的位置和角度
	for (b2Body* pBody = m_pWorld->GetBodyList(); pBody; pBody = pBody->GetNext())
	{
		Character* pCharacter = (Character*)(pBody->GetUserData().pointer);
		if (pCharacter)
		{
			pCharacter->SetPosition({ pBody->GetPosition().x, pBody->GetPosition().y });
			pCharacter->SetAngle(pBody->GetAngle());
		}
	}
}

void GameScene::Render()
{
	SDL_RenderClear(g_pRenderer);

	// 更新摄像头跟随的位置
	if (m_pCamera->m_pMotorJoint && m_pCamera->m_pFollowBody)
	{
		m_pCamera->m_pMotorJoint->SetLinearOffset(m_pCamera->m_pFollowBody->GetPosition());
	}

	GameVec2	drawPoint;
	float		drawAngle;
	SDL_FPoint	drawAnchor;
	SDL_FRect	drawRect;

	for (std::vector<Character*>::iterator iter = m_vecCharacters.begin();
		iter != m_vecCharacters.end(); iter++)
	{
		if (GameImage* pImage = (*iter)->GetImage())
		{
			pImage->Step();

			// 将原图的数据封装为 SDL_Rect
			SDL_Rect sourceRect = {
				0, 0,
				pImage->GetTexture()->m_nWidth,
				pImage->GetTexture()->m_nHeight
			};
			// 将物体的世界坐标换算为在摄像机坐标系下的本地坐标
			drawPoint = ConvertWorldToLocal((*iter)->GetPosition());
			// 将米为单位的距离换算为像素为单位的距离（1.0f 的缩放比例下为 100 像素 / 米）
			drawPoint *= 100 * m_pCamera->m_fZoom;
			// 将其从 Box2D 参考系转换到屏幕坐标系上
			drawPoint.m_x += g_settings.m_nWindowWidth / 2;
			drawPoint.m_y = g_settings.m_nWindowHeight / 2 - drawPoint.m_y;
			// 将数据封装为 SDL 需要的 SDL_FRect
			drawRect =
			{
				drawPoint.m_x - (float)pImage->GetTexture()->m_nWidth / 2,
				drawPoint.m_y - (float)pImage->GetTexture()->m_nHeight / 2,
				(float)pImage->GetTexture()->m_nWidth,
				(float)pImage->GetTexture()->m_nHeight
			};
			// 将 Box2D 的弧度换算为 SDL 所需要的度数
			drawAngle = (*iter)->GetAngle() * (180.0f / b2_pi);
			// 计算出图像的锚点
			drawAnchor = {
				(float)pImage->GetTexture()->m_nWidth / 2,
				(float)pImage->GetTexture()->m_nHeight / 2
			};
			// 判断其是否在摄像机的取景范围内，如果是则绘制该角色
			if (drawPoint.m_x + pImage->GetTexture()->m_nWidth / 2 >= 0 &&
				drawPoint.m_x - pImage->GetTexture()->m_nWidth / 2 <= g_settings.m_nWindowWidth &&
				drawPoint.m_y + pImage->GetTexture()->m_nHeight / 2 >= 0 &&
				drawPoint.m_y - pImage->GetTexture()->m_nHeight / 2 <= g_settings.m_nWindowHeight)
			{
				SDL_RenderCopyExF(g_pRenderer, pImage->GetTexture()->m_pTexture,
					&sourceRect, &drawRect, drawAngle, &drawAnchor,
					(*iter)->GetFlip());
			}
		}
	}
}

void GameScene::CameraFollow(b2Body* body)
{
	b2MotorJointDef motorJointDef;
	motorJointDef.bodyA = m_pGround;
	motorJointDef.bodyB = m_pCamera->m_pBody;
	motorJointDef.maxForce = 4000.0f;
	m_pCamera->m_pFollowBody = body;
	m_pCamera->m_pMotorJoint = (b2MotorJoint*)m_pWorld->CreateJoint(&motorJointDef);
}

const GameVec2& GameScene::ConvertWorldToLocal(const GameVec2& worldPoint)
{
	b2Vec2 vec2 = m_pCamera->m_pBody->GetLocalPoint({ worldPoint.m_x, worldPoint.m_y });
	return { vec2.x, vec2.y };
}

const GameVec2& GameScene::ConvertLocalToWorld(const GameVec2& localPoint)
{
	b2Vec2 vec2 = m_pCamera->m_pBody->GetWorldPoint({ localPoint.m_x, localPoint.m_y });
	return { vec2.x, vec2.y };
}

b2World* GameScene::GetWorld()
{
	return m_pWorld;
}

void GameScene::AddCharacter(Character* pCharacter)
{
	m_vecCharacters.push_back(pCharacter);
	SortCharacters();
}

void GameScene::SortCharacters()
{
	std::sort(m_vecCharacters.begin(), m_vecCharacters.end(),
		[](Character* a, Character* b)->bool { return a->GetZOrder() < b->GetZOrder(); });
}

GameScene::GameScene()
{
	m_pWorld = new b2World(b2Vec2(0.0f, -10.0f));

	// 在原点创建地面刚体
	b2BodyDef groundDef;
	m_pGround = m_pWorld->CreateBody(&groundDef);

	// 摄像机刚体定义
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.fixedRotation = true;
	bodyDef.gravityScale = 0.0f;

	// 创建摄像机
	m_pCamera = new GameCamera();
	m_pCamera->m_fZoom = 1.0f;
	m_pCamera->m_pMotorJoint = nullptr;
	m_pCamera->m_pFollowBody = nullptr;
	m_pCamera->m_pBody = m_pWorld->CreateBody(&bodyDef);

	// 摄像机的质量数据
	b2MassData massData = { 1.0f, {0, 0}, 0 };
	m_pCamera->m_pBody->SetMassData(&massData);
}

GameScene::~GameScene()
{
	if (m_pCamera)
	{
		delete m_pCamera;
	}
	for (std::vector<Character*>::iterator iter = m_vecCharacters.begin();
		iter != m_vecCharacters.end(); iter++)
	{
		delete (*iter);
	}
	if (m_pWorld)
	{
		delete m_pWorld;
	}
}

// SceneManager

int SceneManager::RegisterScene(const char* name, SceneCreateFunc* createFunc)
{
	if (m_nCurrentSceneCount < MAX_SCENE_AMOUNT)
	{
		m_arrSceneEntries[m_nCurrentSceneCount++] = { name, createFunc };
		m_nCurrentSceneCount = m_nCurrentSceneIndex;
		return m_nCurrentSceneIndex;
	}
	return -1;
}

GameScene* SceneManager::GetCurrentScene()
{
	return m_pCurrentScene;
}

GameScene* SceneManager::SwitchTo(int index)
{
	if (m_pCurrentScene)
	{
		delete m_pCurrentScene;
	}
	m_nCurrentSceneIndex = index;
	m_pCurrentScene = m_arrSceneEntries[index].createFunc();
	return m_pCurrentScene;
	return nullptr;
}

SceneManager::SceneManager() :
	m_nCurrentSceneIndex(0), m_nCurrentSceneCount(0), m_pCurrentScene(nullptr)
{
	for (int i = 0; i < MAX_SCENE_AMOUNT; i++)
	{
		m_arrSceneEntries[i] = { nullptr, nullptr };
	}
}

SceneManager::~SceneManager()
{
	if (m_pCurrentScene)
	{
		delete m_pCurrentScene;
	}
}