#ifndef CHARACTER_H_
#define CHARACTER_H_

// STL Support
#include <vector>

// Math Support
#include "game_math.h"

// 角色贴图前置
#include "graphic.h"

// 读取全局设置中的频率
#include "settings.h"

class Character
{
public:
	void					SetPosition(const GameVec2&);
	void					SetAngle(const float&);
	void					SetZOrder(const int&);
	void					SetFlip(const SDL_RendererFlip);

	const GameVec2&			GetPosition();
	const float&			GetAngle();
	const int&				GetZOrder();
	const SDL_RendererFlip	GetFlip();

	b2Body*					CreateBody(b2World*, const b2BodyDef*);
	void					DestroyBody();
	b2Body*					GetBody();

	void					AddImage(GameImage*);
	void					DeleteImage(const int&);
	void					SetImageIndex(const int&);
	GameImage*				GetImage();

private:
	b2Body*					m_pBody;

	GameVec2				m_vec2Position;
	float					m_fAngle;
	int						m_nZOrder;
	SDL_RendererFlip		m_emFlip;

	int						m_nImageIndex;
	std::vector<GameImage*> m_vecImages;

public:
	Character(GameVec2 position = { 0.0f, 0.0f }, float angle = 0.0f, int zOrder = 0);
	Character(b2World* world, const b2BodyDef* bodyDef, int zOrder = 0);
	~Character();
};

#endif