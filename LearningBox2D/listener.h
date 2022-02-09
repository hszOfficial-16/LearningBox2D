#ifndef LISTNER_H_
#define LISTNER_H_

// SDL Support
#include <SDL.h>

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

#endif