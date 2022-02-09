#ifndef SCENE_H_
#define SCENE_H_

// Box2D Support
#include <box2d.h>

// STL Support
#include <queue>
#include <vector>

// 使用角色的数据
#include "character.h"

// 监听事件
#include "listener.h"

struct GameCamera
{
	/// 摄像机的缩放系数
	float			m_fZoom;
	///	摄像机的缩放范围
	GameVec2		m_vec2ZoomLimit;
	/// 摄像机的缩放速度
	float			m_fZoomSpeed;
	/// 摄像机的缩放加速度
	float			m_fZoomAcceleration;
	/// 摄像机在 Box2D 世界中的对应刚体
	b2Body*			m_pBody;
	/// 摄像机跟随的刚体
	b2Body*			m_pFollowBody;
	/// 摄像机在 Box2D 世界中跟随对象的关节
	b2MotorJoint*	m_pMotorJoint;
};

class GameScene
{
public:
	/// 获取 Box2D 世界指针
	b2World*				GetWorld();
	/// 为场景添加角色
	void					AddCharacter(Character*);
	/// 按照角色的Z轴大小为所有角色进行排序
	void					SortCharacters();
	/// 更新场景的函数（负责逻辑）
	virtual void			Step();
	/// 渲染整个场景
	void					Render();
	/// 将世界坐标转换为摄像机本地坐标
	const GameVec2&			ConvertWorldToLocal(const GameVec2&);
	/// 将摄像机本地坐标转换为世界坐标
	const GameVec2&			ConvertLocalToWorld(const GameVec2&);
	/// 让场景摄像机跟随某一实体
	void					CameraFollow(b2Body*);
	/// 改变摄像机的焦距变化状态
	void					SetCameraZoomSpeed(const float&);
	/// 获取摄像机的焦距
	const float&			GetCameraZoom();
private:
	/// 场景对应的 Box2D 世界
	b2World*				m_pWorld;
	/// 场景中的原点静态刚体
	b2Body*					m_pGround;
	/// 场景中的移动摄像机
	GameCamera*				m_pCamera;
	/// 管理场景中所有角色的向量
	std::vector<Character*>	m_vecCharacters;
public:
	GameScene();
	virtual ~GameScene();
};

// 场景管理相关

typedef GameScene* SceneCreateFunc();

struct SceneEntry
{
	const char*			name;
	SceneCreateFunc*	createFunc;
};

class SceneManager
{
public:
	int			RegisterScene(const char*, SceneCreateFunc*);
	GameScene*	GetCurrentScene();
	GameScene*	SwitchTo(int);

private:
	enum		{ MAX_SCENE_AMOUNT = 256 };

	int			m_nCurrentSceneIndex;
	int			m_nCurrentSceneCount;
	GameScene*	m_pCurrentScene;
	SceneEntry	m_arrSceneEntries[MAX_SCENE_AMOUNT];

public:
	~SceneManager();
	SceneManager(const SceneManager&) = delete;
	SceneManager& operator=(const SceneManager&) = delete;
	static SceneManager& GetInstance()
	{
		static SceneManager instance;
		return instance;
	}
private:
	SceneManager();
};

#endif