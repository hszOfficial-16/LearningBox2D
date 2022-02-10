#include "listener.h"

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
	delete m_vecListeners[index];
	Listener* swapListener = m_vecListeners[m_vecListeners.size() - 1];
	m_vecListeners[index] = swapListener;
	m_vecListeners.pop_back();
}

void ListenerManager::UnregisterStatic(size_t index)
{
	delete m_vecStaticListeners[index];
	Listener* swapListener = m_vecStaticListeners[m_vecStaticListeners.size() - 1];
	m_vecStaticListeners[index] = swapListener;
	m_vecStaticListeners.pop_back();
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

Uint32 ContactListener::GetEventType()
{
	return m_eventType;
}

void ContactListener::BeginContact(b2Contact* contact)
{
	m_contactEvent.user.code = (Sint32)ContactType::BEGIN_CONTACT;
	m_contactEvent.user.data1 = (void*)contact;
	m_contactEvent.user.data2 = nullptr;
	SDL_PushEvent(&m_contactEvent);
}

void ContactListener::EndContact(b2Contact* contact)
{
	m_contactEvent.user.code = (Sint32)ContactType::END_CONTACT;
	m_contactEvent.user.data1 = (void*)contact;
	m_contactEvent.user.data2 = nullptr;
	SDL_PushEvent(&m_contactEvent);
}

void ContactListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
{
	m_contactEvent.user.code = (Sint32)ContactType::PRESOLVE;
	m_contactEvent.user.data1 = (void*)contact;
	m_contactEvent.user.data2 = (void*)oldManifold;
	SDL_PushEvent(&m_contactEvent);
}

void ContactListener::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
{
	m_contactEvent.user.code = (Sint32)ContactType::PRESOLVE;
	m_contactEvent.user.data1 = (void*)contact;
	m_contactEvent.user.data2 = (void*)impulse;
	SDL_PushEvent(&m_contactEvent);
}

ContactListener::ContactListener()
{
	m_eventType = SDL_RegisterEvents(1);
	if (m_eventType != ((Uint32)-1))
	{
		SDL_memset(&m_contactEvent, 0, sizeof(m_contactEvent));
		m_contactEvent.type = m_eventType;
	}
}