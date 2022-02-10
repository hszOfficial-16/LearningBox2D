#include "main.h"
using namespace std;

void InitGame()
{
	// SDL 初始化
	SDL_Init(SDL_INIT_EVERYTHING);
	g_pWindow = SDL_CreateWindow("Box2D's test",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		g_settings.m_nWindowWidth, g_settings.m_nWindowHeight, SDL_WINDOW_SHOWN);
	g_pRenderer =
		SDL_CreateRenderer(g_pWindow, -1, SDL_RENDERER_ACCELERATED);
	SDL_SetRenderDrawBlendMode(g_pRenderer, SDL_BLENDMODE_BLEND);
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "best");
}

void RegisterEssentialListeners()
{
	Listener* listenQuit = nullptr;
	{
		listenQuit = new Listener(SDL_QUIT, [](SDL_Event* event) { g_settings.m_bIsQuit = true; });
		ListenerManager::GetInstance().RegisterStatic(listenQuit);
	}
}

void QuitGame()
{
	// 安全退出
	SDL_DestroyRenderer(g_pRenderer);
	g_pRenderer = nullptr;
	SDL_DestroyWindow(g_pWindow);
	g_pWindow = nullptr;
	SDL_Quit();
}

#undef main
int main(int argc, char** argv)
{
	// 初始化各种库
	InitGame();

	// 注册所有基础的监听器（静态）
	RegisterEssentialListeners();

	std::chrono::duration<double> dFrameTime(0.0);
	std::chrono::duration<double> dSleepAdjust(0.0);

	GameScene* currentScene = SceneManager::GetInstance().SwitchTo(0);

	while (!g_settings.m_bIsQuit)
	{
		std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();

		// 管理器向所有监听者分发事件
		ListenerManager::GetInstance().NotifyAll();

		// 进行该帧的逻辑更新
		currentScene->Step();

		// 渲染该帧的图像
		currentScene->Render();

		// 算出应当休眠的时间
		std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now();
		std::chrono::duration<double> dTarget(1.0 / g_settings.m_fHertz);
		std::chrono::duration<double> dTimeUsed = t2 - t1;
		std::chrono::duration<double> dSleepTime = dTarget - dTimeUsed + dSleepAdjust;
		if (dSleepTime > std::chrono::duration<double>(0))
		{
			std::this_thread::sleep_for(dSleepTime);
		}
		// 交换缓冲区，展示该帧画面
		SDL_RenderPresent(g_pRenderer);

		// 根据直到当前的实际帧数和预期帧数的差来调整下一帧的休眠时间
		std::chrono::steady_clock::time_point t3 = std::chrono::steady_clock::now();
		dFrameTime = t3 - t1;
		dSleepAdjust = 0.9 * dSleepAdjust + 0.1 * (dTarget - dFrameTime);
	}

	QuitGame();
	return 0;
}