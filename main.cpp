#include <SDL2\SDL.h>
#include <SDL2\SDL_image.h>
#include <stdio.h>
#include <string>
using namespace std;

// �������泣��
enum KeyPressSurfaces
{
	KEY_PRESS_SURFACE_DEFAULT,
	KEY_PRESS_SURFACE_UP,
	KEY_PRESS_SURFACE_DOWN,
	KEY_PRESS_SURFACE_LEFT,
	KEY_PRESS_SURFACE_RIGHT,
	KEY_PRESS_SURFACE_TOTAL
};

// ��������
bool init();
bool loadMedia();
void close();
SDL_Surface *loadSurface(const string &path);

// ���ڴ�С
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

// ����ָ��
SDL_Window *gWindow = NULL;
// ���ڱ��棬���ڻ��ƣ��Ǵ��ڵ�һ���������򣬿�����������ͼ���ı���
SDL_Surface *gScreenSurface = NULL;
SDL_Surface *gKeyPressSurfaces[KEY_PRESS_SURFACE_TOTAL];
SDL_Surface *gCurrentSurface = NULL;

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
	// ��ʼ�� SDL_image ��
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
		return false;
	}

	// ��ȡ���ڱ���
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
	// ���ٴ���
	SDL_DestroyWindow(gWindow);
	// �˳�SDL��ϵͳ
	SDL_Quit();
}

SDL_Surface *loadSurface(const string &path)
{

	SDL_Surface *optimizedSurface = NULL;
	// ����ͼƬ
	SDL_Surface *loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		printf("Unable to load image %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		return NULL;
	}
	// ת��ͼƬ��ʽ
	optimizedSurface = SDL_ConvertSurface(loadedSurface, gScreenSurface->format, 0);
	if (optimizedSurface == NULL)
	{
		printf("Unable to optimize image %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		return NULL;
	}
	// �ͷ�ԭͼƬ
	SDL_FreeSurface(loadedSurface);
	return optimizedSurface;
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
		}

		// ��䴰�ڱ���
		SDL_Rect stretchRect;
		stretchRect.x = 0;
		stretchRect.y = 0;
		stretchRect.w = SCREEN_WIDTH;
		stretchRect.h = SCREEN_HEIGHT;
		SDL_BlitScaled(gCurrentSurface, NULL, gScreenSurface, &stretchRect);

		// ���´��ڱ���
		SDL_UpdateWindowSurface(gWindow);
	}
	close();
	return 0;
}