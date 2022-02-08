#include "settings.h"

Settings g_settings;

void Settings::Reset()
{
	m_nWindowWidth = 800;
	m_nWindowHeight = 600;
	m_fHertz = 60.0f;
	m_nVelocityIterations = 8;
	m_nPositionIterations = 3;
	m_bIsQuit = false;
}

void Settings::Save()
{

}

void Settings::Load()
{

}

Settings::Settings()
{
	Reset();
}