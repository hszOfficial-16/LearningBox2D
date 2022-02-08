#ifndef GAME_MATH_H_
#define GAME_MATH_H_

// SDL Support
#include <SDL.h>

// Box2D Support
#include <box2d.h>

// Math Support
#include <cmath>

struct GameVec2
{
	void Set(float x, float y)
	{
		m_x = x; m_y = y;
	}

	// ����һ������
	void operator += (const GameVec2& v)
	{
		m_x += v.m_x; m_y += v.m_y;
	}

	// ��ȥһ������
	void operator -= (const GameVec2& v)
	{
		m_x -= v.m_x; m_y -= v.m_y;
	}

	// ����һ������
	void operator *= (float a)
	{
		m_x *= a; m_y *= a;
	}

	operator SDL_FPoint()
	{
		return { m_x, m_y };
	}
	
	float m_x, m_y;
};

#endif // !GAME_MATH_H_