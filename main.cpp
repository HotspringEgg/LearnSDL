#include <SDL2\SDL.h>
#include <SDL2\SDL_image.h>
#include <stdio.h>
#include <string>
using namespace std;

// 按键表面常量
enum KeyPressSurfaces
{
	KEY_PRESS_SURFACE_DEFAULT,
	KEY_PRESS_SURFACE_UP,
	KEY_PRESS_SURFACE_DOWN,
	KEY_PRESS_SURFACE_LEFT,
	KEY_PRESS_SURFACE_RIGHT,
	KEY_PRESS_SURFACE_TOTAL
};

// 函数声明
bool init();
bool loadMedia();
void close();
SDL_Surface *loadSurface(const string &path);

// 窗口大小
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

// 窗口指针
SDL_Window *gWindow = NULL;
// 窗口表面，用于绘制，是窗口的一个矩形区域，可以用来绘制图像，文本等
SDL_Surface *gScreenSurface = NULL;
SDL_Surface *gKeyPressSurfaces[KEY_PRESS_SURFACE_TOTAL];
SDL_Surface *gCurrentSurface = NULL;

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
	// 初始化 SDL_image 库
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
		return false;
	}

	// 获取窗口表面
	gScreenSurface = SDL_GetWindowSurface(gWindow);
	return true;
}

bool loadMedia()
{
	// Loading success flag
	bool success = true;

	// Load stretching surface
	gCurrentSurface = loadSurface("loaded.png");
	if (gCurrentSurface == NULL)
	{
		printf("Failed to load stretching image!\n");
		success = false;
	}
	return success;
}

void close()
{
	// 销毁窗口
	SDL_DestroyWindow(gWindow);
	// 退出SDL子系统
	SDL_Quit();
}

SDL_Surface *loadSurface(const string &path)
{

	SDL_Surface *optimizedSurface = NULL;
	// 加载图片
	SDL_Surface *loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		printf("Unable to load image %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		return NULL;
	}
	// 转换图片格式
	optimizedSurface = SDL_ConvertSurface(loadedSurface, gScreenSurface->format, 0);
	if (optimizedSurface == NULL)
	{
		printf("Unable to optimize image %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		return NULL;
	}
	// 释放原图片
	SDL_FreeSurface(loadedSurface);
	return optimizedSurface;
}

int main(int argc, char *args[])
{
	// 初始化窗口
	init();

	// 加载媒体资源
	loadMedia();

	SDL_Event e;
	bool quit = false;

	while (!quit)
	{
		// 处理事件
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}
		}

		// 填充窗口表面
		SDL_Rect stretchRect;
		stretchRect.x = 0;
		stretchRect.y = 0;
		stretchRect.w = SCREEN_WIDTH;
		stretchRect.h = SCREEN_HEIGHT;
		SDL_BlitScaled(gCurrentSurface, NULL, gScreenSurface, &stretchRect);

		// 更新窗口表面
		SDL_UpdateWindowSurface(gWindow);
	}
	close();
	return 0;
}