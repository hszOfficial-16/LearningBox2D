#include "listener.h"

#include <iostream>

// Listener

void Listener::Listen(Uint32 listenType, std::function<void(SDL_Event*)> callback)
{
	m_emListenType = listenType;
	m_funcCallback = callback;
}

Listener::Listener():
	m_emListenType(SDL_FIRSTEVENT), m_nIndex(sizeof(size_t) - 1)
{
}

Listener::Listener(Uint32 listenType, std::function<void(SDL_Event*)> callback):
	m_emListenType(listenType), m_funcCallback(callback), m_nIndex(sizeof(size_t) - 1)
{
}

Listener::~Listener()
{
	if (m_nIndex != sizeof(size_t) - 1 && m_emListenerType != Type::STATIC)
	{
		ListenerManager::GetInstance().Unregister(m_nIndex);
	}
}

// Listener Manager

void ListenerManager::Register(Listener* listener)
{
	m_vecListeners.push_back(listener);
	listener->m_emListenerType = Listener::Type::NORMAL;
	listener->m_nIndex = m_vecListeners.size() - 1;
}

void ListenerManager::RegisterStatic(Listener* listener)
{
	m_vecStaticListeners.push_back(listener);
	listener->m_emListenerType = Listener::Type::STATIC;
	listener->m_nIndex = m_vecListeners.size() - 1;
}

void ListenerManager::Unregister(size_t index)
{
	Listener* swapListener = m_vecListeners[m_vecListeners.size() - 1];
	m_vecListeners[index] = swapListener;
	m_vecListeners.pop_back();
}

void ListenerManager::NotifyAll()
{
	SDL_Event theEvent;
	while (SDL_PollEvent(&theEvent))
	{
		for (std::vector<Listener*>::iterator iter = m_vecListeners.begin();
			iter != m_vecListeners.end(); iter++)
		{
			if (theEvent.type == (*iter)->m_emListenType)
			{
				(*iter)->m_funcCallback(&theEvent);
			}
		}
		for (std::vector<Listener*>::iterator iter = m_vecStaticListeners.begin();
			iter != m_vecStaticListeners.end(); iter++)
		{
			if (theEvent.type == (*iter)->m_emListenType)
			{
				(*iter)->m_funcCallback(&theEvent);
			}
		}
	}
}

void ListenerManager::Clear()
{
	for (std::vector<Listener*>::iterator iter = m_vecListeners.begin();
		iter != m_vecListeners.end(); iter++)
	{
		delete (*iter);
	}
}

ListenerManager::~ListenerManager()
{
	for (std::vector<Listener*>::iterator iter = m_vecListeners.begin();
		iter != m_vecListeners.end(); iter++)
	{
		delete (*iter);
	}
	for (std::vector<Listener*>::iterator iter = m_vecStaticListeners.begin();
		iter != m_vecStaticListeners.end(); iter++)
	{
		delete (*iter);
	}
}