#ifndef GRAPHIC_H_
#define GRAPHIC_H_

// SDL Support
#include <SDL.h>

// STL Support
#include <vector>
#include <string>
#include <filesystem>
#include <unordered_map>
#include <initializer_list>

struct GameTexture
{
	int				m_nWidth;
	int				m_nHeight;
	SDL_Texture*	m_pTexture;
};

class TextureManager
{
public:
	GameTexture*	LoadTexture(const std::string&);
	void			UnloadTexture(const std::string&);

private:
	std::unordered_map<std::string, GameTexture*> m_mapTextures;

public:
	~TextureManager();
	TextureManager(const TextureManager&) = delete;
	TextureManager& operator=(const TextureManager&) = delete;
	static TextureManager& GetInstance()
	{
		static TextureManager instance;
		return instance;
	}
private:
	TextureManager() = default;
};

struct GameFrame
{
	std::string		m_strFileName;
	unsigned int	m_nDuration;
};

class GameImage
{
public:
	void					Step();
	void					SetDefault();
	GameTexture*			GetTexture();

private:
	size_t					m_nIndex;
	unsigned int			m_nProgress;
	std::vector<GameFrame>	m_vecFrames;

public:
	GameImage(std::string);
	GameImage(std::initializer_list<GameFrame>);
};

extern SDL_Window*		g_pWindow;
extern SDL_Renderer*	g_pRenderer;

#endif // !GRAPHIC_H_