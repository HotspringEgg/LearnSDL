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

// ����ָ��
SDL_Window *gWindow = NULL;
// ���ڱ��棬���ڻ��ƣ��Ǵ��ڵ�һ���������򣬿�����������ͼ���ı���
SDL_Surface *gScreenSurface = NULL;
SDL_Surface *gKeyPressSurfaces[KEY_PRESS_SURFACE_TOTAL];
SDL_Surface *gCurrentSurface = NULL;

// �������泣��

bool init()
{
	// ��ʼ��SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	// ��������
	gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (gWindow == NULL)
	{
		printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	// ��ȡ���ڱ���
	gScreenSurface = SDL_GetWindowSurface(gWindow);
	return true;
}

void close()
{
	// ���ٴ���
	SDL_DestroyWindow(gWindow);
	// �˳�SDL��ϵͳ
	SDL_Quit();
}

SDL_Surface *loadSurface(const string &path)
{
	// ����ͼƬ
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
	// ������Ը��ѭ��������ö�����ͣ��������е�ͼƬ
	// ��Ҫ�ĵĵط��ǣ�loadSurface�����Ĳ�����һ����ö�����ͣ�һ����ͼƬ·��
	// ����Ĭ��ͼƬ
	gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT] = loadSurface("press.bmp");
	if (gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT] == NULL)
	{
		printf("Failed to load default image!\n");
		return false;
	}

	// ������ͼƬ
	gKeyPressSurfaces[KEY_PRESS_SURFACE_UP] = loadSurface("up.bmp");
	if (gKeyPressSurfaces[KEY_PRESS_SURFACE_UP] == NULL)
	{
		printf("Failed to load up image!\n");
		return false;
	}

	// ������ͼƬ
	gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN] = loadSurface("down.bmp");
	if (gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN] == NULL)
	{
		printf("Failed to load down image!\n");
		return false;
	}

	// ������ͼƬ
	gKeyPressSurfaces[KEY_PRESS_SURFACE_LEFT] = loadSurface("left.bmp");
	if (gKeyPressSurfaces[KEY_PRESS_SURFACE_LEFT] == NULL)
	{
		printf("Failed to load left image!\n");
		return false;
	}

	// ������ͼƬ
	gKeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT] = loadSurface("right.bmp");
	if (gKeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT] == NULL)
	{
		printf("Failed to load right image!\n");
		return false;
	}

	// ���õ�ǰ��ʾ��ͼƬ
	gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT];
	return true;
}

int main(int argc, char *args[])
{
	// ��ʼ������
	init();

	// ����ý����Դ
	loadMedia();

	SDL_Event e;
	bool quit = false;

	while (!quit)
	{
		// �����¼�
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

		// �ں�ͼƬ
		SDL_BlitSurface(gCurrentSurface, NULL, gScreenSurface, NULL);
		// ���´��ڱ���
		SDL_UpdateWindowSurface(gWindow);
	}
	close();
	return 0;
}