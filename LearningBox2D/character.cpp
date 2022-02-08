#include "character.h"

// Character

void Character::SetPosition(const GameVec2& position)
{
	m_vec2Position = position;
}

void Character::SetAngle(const float& angle)
{
	m_fAngle = angle;
}

void Character::SetZOrder(const int& zOrder)
{
	m_nZOrder = zOrder;
}

void Character::SetFlip(const SDL_RendererFlip flip)
{
	m_emFlip = flip;
}

void Character::SetImageIndex(const int& index)
{
	m_nImageIndex = index;
}

b2Body* Character::GetBody()
{
	return m_pBody;
}

const GameVec2& Character::GetPosition()
{
	return m_vec2Position;
}

const float& Character::GetAngle()
{
	return m_fAngle;
}

const int& Character::GetZOrder()
{
	return m_nZOrder;
}

const SDL_RendererFlip Character::GetFlip()
{
	return m_emFlip;
}

GameImage* Character::GetImage()
{
	if (m_nImageIndex < m_vecImages.size())
	{
		return m_vecImages[m_nImageIndex];
	}
	return nullptr;
}

b2Body* Character::CreateBody(b2World* world, const b2BodyDef* bodyDef)
{
	if (!m_pBody)
	{
		m_pBody = world->CreateBody(bodyDef);
		m_pBody->GetUserData().pointer = (uintptr_t)this;
	}
	else
	{
		SDL_ShowSimpleMessageBox(
			SDL_MESSAGEBOX_ERROR,
			"Error Creating",
			"Try to create body on a character that has a body.",
			nullptr
		);
	}
}

void Character::DestroyBody()
{
	if (m_pBody)
	{
		m_pBody->GetWorld()->DestroyBody(m_pBody);
		m_pBody = nullptr;
	}
	else
	{
		SDL_ShowSimpleMessageBox(
			SDL_MESSAGEBOX_ERROR,
			"Error Destroying",
			"Try to destroy body on a character that hasn't a body.",
			nullptr
		);
	}
}

void Character::AddImage(GameImage* image)
{
	m_vecImages.push_back(image);
}

void Character::DeleteImage(const int& index)
{
	std::vector<GameImage*>::iterator iter = m_vecImages.begin() + index;
	m_vecImages.erase(iter);
	delete (*iter);
}

Character::Character(GameVec2 position, float angle, int zOrder) :
	m_vec2Position(position), m_fAngle(angle), m_nZOrder(zOrder),
	m_pBody(nullptr), m_emFlip(SDL_FLIP_NONE), m_nImageIndex(0)
{
}

Character::Character(b2World* world, const b2BodyDef* bodyDef, int zOrder) :
	m_nZOrder(zOrder), m_emFlip(SDL_FLIP_NONE), m_nImageIndex(0)
{
	m_vec2Position = { bodyDef->position.x, bodyDef->position.y };
	m_fAngle = bodyDef->angle;
	m_pBody = world->CreateBody(bodyDef);
	m_pBody->GetUserData().pointer = (uintptr_t)this;
}

Character::~Character()
{
	if (m_pBody)
	{
		m_pBody->GetWorld()->DestroyBody(m_pBody);
		m_pBody = nullptr;
	}
	for (std::vector<GameImage*>::iterator iter = m_vecImages.begin();
		iter != m_vecImages.end(); iter++)
	{
		delete (*iter);
	}
}