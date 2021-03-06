#ifndef LISTNER_H_
#define LISTNER_H_

// SDL Support
#include <SDL.h>

// Box2D Support
#include <box2d.h>

// STL Container
#include <vector>

// C++11 Function
#include <functional>

class Listener
{
public:
	void							Listen(Uint32, std::function<void(SDL_Event*)>);

private:
	enum class Type
	{
		NORMAL = 0,
		STATIC
	};

	size_t							m_nIndex;
	Uint32							m_emListenType;
	Type							m_emListenerType;
	std::function<void(SDL_Event*)>	m_funcCallback;

public:
	Listener();
	Listener(Uint32, std::function<void(SDL_Event*)>);
	~Listener() = default;

	friend class ListenerManager;
};

class ListenerManager
{
public:
	/// 注册一个监听器，并且可以将其注销
	void					Register(Listener*);
	/// 注册一个静态监听器，不可被注销
	void					RegisterStatic(Listener*);
	/// 注销一个非静态监听器
	void					Unregister(size_t);
	/// 注销一个静态监听器
	void					UnregisterStatic(size_t);
	/// 读取当前所有的事件并分发给所有监听者
	void					NotifyAll();
	/// 清空所有非静态监听器
	void					Clear();

private:
	std::vector<Listener*>	m_vecListeners;
	// 用于管理静态监听者，他们负责游戏中基本的事件监听
	std::vector<Listener*>  m_vecStaticListeners;
public:
	~ListenerManager();
	ListenerManager(const ListenerManager&) = delete;
	ListenerManager& operator=(const ListenerManager&) = delete;
	static ListenerManager& GetInstance()
	{
		static ListenerManager instance;
		return instance;
	}
private:
	ListenerManager() = default;
};

// 接收 Box2D 的消息并将其广播到 SDL 的事件队列中去
enum class ContactType
{
	BEGIN_CONTACT = 0,
	END_CONTACT,
	PRESOLVE,
	POSTSOLVE
};

class ContactListener : public b2ContactListener
{
public:
	Uint32	GetEventType();

	void	BeginContact(b2Contact*) override;
	void	EndContact(b2Contact*) override;
	void	PreSolve(b2Contact*, const b2Manifold*) override;
	void	PostSolve(b2Contact*, const b2ContactImpulse*) override;

private:
	Uint32		m_eventType;
	SDL_Event	m_contactEvent;

public:
	~ContactListener() = default;
	ContactListener(const ContactListener&) = delete;
	ContactListener& operator=(const ContactListener&) = delete;
	static ContactListener& GetInstance()
	{
		static ContactListener instance;
		return instance;
	}
private:
	ContactListener();
};

#endif