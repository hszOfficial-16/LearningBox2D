#include "graphic.h"

#include <SDL_image.h>

// TextureManager

GameTexture* TextureManager::LoadTexture(const std::string& file)
{
	if (m_mapTextures[file])
	{
		return m_mapTextures[file];
	}
	else
	{
		SDL_Surface* pSurface = IMG_Load(file.c_str());

		if (pSurface)
		{
			SDL_Texture* pTexture = SDL_CreateTextureFromSurface(g_pRenderer, pSurface);
			GameTexture* pGameTexture = new GameTexture({ pSurface->w, pSurface->h, pTexture });
			m_mapTextures[file] = pGameTexture;
			return pGameTexture;
		}
		else
		{
			return nullptr;
		}
	}
}

void TextureManager::UnloadTexture(const std::string& file)
{
	if (m_mapTextures[file])
	{
		m_mapTextures.erase(file);
	}
	else
	{
		SDL_ShowSimpleMessageBox(
			SDL_MESSAGEBOX_ERROR,
			"Error Unloading",
			"An error occured when unloading a texture that is nullptr",
			nullptr
		);
	}
}

TextureManager::~TextureManager()
{
	for (std::unordered_map<std::string, GameTexture*>::iterator iter = m_mapTextures.begin();
		iter != m_mapTextures.end(); iter++)
	{
		SDL_DestroyTexture((*iter).second->m_pTexture);
		delete (*iter).second;
	}
}

// GameImage

void GameImage::Step()
{
	if (m_vecFrames[m_nIndex].m_nDuration != 0)
	{
		if (m_nProgress++ >= m_vecFrames[m_nIndex].m_nDuration)
		{
			m_nProgress = 0;
			m_nIndex = (m_nIndex + 1) % m_vecFrames.size();
		}
	}
}

void GameImage::SetDefault()
{
	m_nProgress = 0;
	m_nIndex = 0;
}

GameTexture* GameImage::GetTexture()
{
	return TextureManager::GetInstance().LoadTexture(m_vecFrames[m_nIndex].m_strFileName);
}

void GameImage::IncreaseCount()
{
	m_nReferenceCount++;
}

void GameImage::ReduceCount()
{
	if (--m_nReferenceCount <= 0)
	{
		GameImage::~GameImage();
	}
}

GameImage::GameImage(std::string file) :
	m_nIndex(0), m_nProgress(0), m_nReferenceCount(0)
{
	m_vecFrames.push_back({ file, 0 });
}

GameImage::GameImage(std::initializer_list<GameFrame> ilFrames) :
	m_nIndex(0), m_nProgress(0), m_nReferenceCount(0)
{
	for (std::initializer_list<GameFrame>::iterator iter = ilFrames.begin();
		iter != ilFrames.end(); iter++)
	{
		m_vecFrames.push_back(*iter);
	}
}

SDL_Window*		g_pWindow	= nullptr;
SDL_Renderer*	g_pRenderer = nullptr;