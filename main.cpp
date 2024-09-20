#include <SDL2\SDL.h>
#include <stdio.h>
#include <string>
using namespace std;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

// ����ָ��
SDL_Window *gWindow = NULL;
// ���ڱ��棬���ڻ��ƣ��Ǵ��ڵ�һ���������򣬿�����������ͼ���ı���
SDL_Surface *gScreenSurface = NULL;
SDL_Surface *gHelloWorld = NULL;

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

bool loadMedia(const string &path, SDL_Surface *&targetSurface)
{
	// ���Ŀ������Ѿ������ݣ����ͷ�
	if (targetSurface != NULL)
	{
		SDL_FreeSurface(targetSurface);
		targetSurface = NULL;
	}

	// ����ͼ��Ŀ�����
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
	// ���ٴ���
	SDL_DestroyWindow(gWindow);
	// �˳�SDL��ϵͳ
	SDL_Quit();
}

int main(int argc, char *args[])
{
	// ��ʼ������
	init();

	// ����ý����Դ
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
		// ���ͼ�񵽴��ڱ���
		SDL_BlitSurface(gHelloWorld, NULL, gScreenSurface, NULL);
		// ���´��ڱ���
		SDL_UpdateWindowSurface(gWindow);
	}
	close();
	return 0;
}