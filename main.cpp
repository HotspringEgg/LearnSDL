#include <SDL2\SDL.h>
#include <stdio.h>
#include <string>
using namespace std;

enum KeyPressSurfaces
{
	KEY_PRESS_SURFACE_DEFAULT,
	KEY_PRESS_SURFACE_UP,
	KEY_PRESS_SURFACE_DOWN,
	KEY_PRESS_SURFACE_LEFT,
	KEY_PRESS_SURFACE_RIGHT,
	KEY_PRESS_SURFACE_TOTAL
};

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

// 窗口指针
SDL_Window *gWindow = NULL;
// 窗口表面，用于绘制，是窗口的一个矩形区域，可以用来绘制图像，文本等
SDL_Surface *gScreenSurface = NULL;
SDL_Surface *gKeyPressSurfaces[KEY_PRESS_SURFACE_TOTAL];
SDL_Surface *gCurrentSurface = NULL;

// 按键表面常量

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

void close()
{
	// 销毁窗口
	SDL_DestroyWindow(gWindow);
	// 退出SDL子系统
	SDL_Quit();
}

SDL_Surface *loadSurface(const string &path)
{
	// 加载图片
	SDL_Surface *loadedSurface = SDL_LoadBMP(path.c_str());
	if (loadedSurface == NULL)
	{
		printf("Unable to load image %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		return NULL;
	}
	return loadedSurface;
}

bool loadMedia()
{
	// 这里可以搞个循环，遍历枚举类型，加载所有的图片
	// 需要改的地方是，loadSurface函数的参数，一个是枚举类型，一个是图片路径
	// 加载默认图片
	gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT] = loadSurface("press.bmp");
	if (gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT] == NULL)
	{
		printf("Failed to load default image!\n");
		return false;
	}

	// 加载上图片
	gKeyPressSurfaces[KEY_PRESS_SURFACE_UP] = loadSurface("up.bmp");
	if (gKeyPressSurfaces[KEY_PRESS_SURFACE_UP] == NULL)
	{
		printf("Failed to load up image!\n");
		return false;
	}

	// 加载下图片
	gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN] = loadSurface("down.bmp");
	if (gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN] == NULL)
	{
		printf("Failed to load down image!\n");
		return false;
	}

	// 加载左图片
	gKeyPressSurfaces[KEY_PRESS_SURFACE_LEFT] = loadSurface("left.bmp");
	if (gKeyPressSurfaces[KEY_PRESS_SURFACE_LEFT] == NULL)
	{
		printf("Failed to load left image!\n");
		return false;
	}

	// 加载右图片
	gKeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT] = loadSurface("right.bmp");
	if (gKeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT] == NULL)
	{
		printf("Failed to load right image!\n");
		return false;
	}

	// 设置当前显示的图片
	gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT];
	return true;
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
			else if (e.type == SDL_KEYDOWN)
			{
				switch (e.key.keysym.sym)
				{
				case SDLK_UP:
					gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_UP];
					break;
				case SDLK_DOWN:
					gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN];
					break;
				case SDLK_LEFT:
					gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_LEFT];
					break;
				case SDLK_RIGHT:
					gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT];
					break;
				default:
					gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT];
					break;
				}
			}
		}

		// 融合图片
		SDL_BlitSurface(gCurrentSurface, NULL, gScreenSurface, NULL);
		// 更新窗口表面
		SDL_UpdateWindowSurface(gWindow);
	}
	close();
	return 0;
}