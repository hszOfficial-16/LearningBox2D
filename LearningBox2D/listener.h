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
	/// ע��һ�������������ҿ��Խ���ע��
	void					Register(Listener*);
	/// ע��һ����̬�����������ɱ�ע��
	void					RegisterStatic(Listener*);
	/// ע��һ���Ǿ�̬������
	void					Unregister(size_t);
	/// ע��һ����̬������
	void					UnregisterStatic(size_t);
	/// ��ȡ��ǰ���е��¼����ַ������м�����
	void					NotifyAll();
	/// ������зǾ�̬������
	void					Clear();

private:
	std::vector<Listener*>	m_vecListeners;
	// ���ڹ���̬�����ߣ����Ǹ�����Ϸ�л������¼�����
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

// ���� Box2D ����Ϣ������㲥�� SDL ���¼�������ȥ
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