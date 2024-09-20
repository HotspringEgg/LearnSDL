#include <SDL2\SDL.h>
#include <stdio.h>
#include <string>
using namespace std;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

// 窗口指针
SDL_Window *gWindow = NULL;
// 窗口表面，用于绘制，是窗口的一个矩形区域，可以用来绘制图像，文本等
SDL_Surface *gScreenSurface = NULL;
SDL_Surface *gHelloWorld = NULL;

bool init()
{
	// 初始化SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	// 创建窗口
	gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (gWindow == NULL)
	{
		printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	// 获取窗口表面
	gScreenSurface = SDL_GetWindowSurface(gWindow);
	return true;
}

bool loadMedia(const string &path, SDL_Surface *&targetSurface)
{
	// 如果目标表面已经有内容，先释放
	if (targetSurface != NULL)
	{
		SDL_FreeSurface(targetSurface);
		targetSurface = NULL;
	}

	// 加载图像到目标表面
	targetSurface = SDL_LoadBMP(path.c_str());
	if (targetSurface == NULL)
	{
		printf("Unable to load image %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		return false;
	}
	return true;
}

void close()
{
	// 销毁窗口
	SDL_DestroyWindow(gWindow);
	// 退出SDL子系统
	SDL_Quit();
}

int main(int argc, char *args[])
{
	// 初始化窗口
	init();

	// 加载媒体资源
	string path = "hello_world.bmp";
	loadMedia(path, gHelloWorld);

	SDL_Event e;
	bool quit = false;

	while (quit == false)
	{
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
				quit = true;
		}
		// 混合图像到窗口表面
		SDL_BlitSurface(gHelloWorld, NULL, gScreenSurface, NULL);
		// 更新窗口表面
		SDL_UpdateWindowSurface(gWindow);
	}
	close();
	return 0;
}