#ifndef SETTINGS_H_
#define SETTINGS_H_

struct Settings
{
	void	Reset();
	void	Save();
	void	Load();

	int		m_nWindowWidth;
	int		m_nWindowHeight;
	float	m_fHertz;
	int		m_nVelocityIterations;
	int		m_nPositionIterations;

	bool	m_bIsQuit;

	Settings();
};

extern Settings g_settings;

#endif // !SETTINGS_H_