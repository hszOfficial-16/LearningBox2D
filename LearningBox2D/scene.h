#ifndef SCENE_H_
#define SCENE_H_

// Box2D Support
#include <box2d.h>

// STL Support
#include <queue>
#include <vector>
#include <algorithm>

// ʹ�ý�ɫ������
#include "character.h"

struct GameCamera
{
	/// ����ͷ������ϵ��
	float			m_fZoom;
	/// ����ͷ�� Box2D �����еĶ�Ӧ����
	b2Body*			m_pBody;
	/// ����ͷ����ĸ���
	b2Body*			m_pFollowBody;
	/// ����ͷ�� Box2D �����и������Ĺؽ�
	b2MotorJoint*	m_pMotorJoint;
};

class GameScene
{
public:
	/// ��ȡ Box2D ����ָ��
	b2World*				GetWorld();
	/// Ϊ�������ӽ�ɫ
	void					AddCharacter(Character*);
	/// ���ս�ɫ��Z���СΪ���н�ɫ��������
	void					SortCharacters();
	/// ���³����ĺ����������߼���
	virtual void			Step();
	/// ��Ⱦ��������
	void					Render();
	/// �ó������������ĳһʵ��
	void					CameraFollow(b2Body*);
	/// ����������ת��Ϊ�������������
	const GameVec2&			ConvertWorldToLocal(const GameVec2&);
	/// ���������������ת��Ϊ��������
	const GameVec2&			ConvertLocalToWorld(const GameVec2&);
private:
	/// ������Ӧ�� Box2D ����
	b2World*				m_pWorld;
	/// �����е�ԭ�㾲̬����
	b2Body*					m_pGround;
	/// �����е��ƶ�����ͷ
	GameCamera*				m_pCamera;
	/// �������������н�ɫ������
	std::vector<Character*>	m_vecCharacters;
public:
	GameScene();
	virtual ~GameScene();
};

// �����������

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