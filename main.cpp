#include <SDL2\SDL.h>
#include <SDL2\SDL_image.h>
#include <stdio.h>
#include <string>
using namespace std;

// ���ڴ�С
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

// ��������
bool init();
bool loadMedia();
void close();
SDL_Texture* loadTexture(const string& path);


// ����ָ��
SDL_Window *gWindow = NULL;
SDL_Renderer *gRenderer = NULL;
SDL_Texture *gTexture = NULL;

bool init()
{
	// ��ʼ��SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
	{
		printf("Warning: Linear texture filtering not enabled!");
		return false;
	}

	// ��������
	gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (gWindow == NULL)
	{
		printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	// ������Ⱦ��
	gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
	if (gRenderer == NULL)
	{
		printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
		return false;
	}
	// ��ʼ����Ⱦ����ɫ
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

	// ��ʼ�� SDL_image ��
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
		return false;
	}
	return true;
}

bool loadMedia()
{
	// Loading success flag
	bool success = true;

	// Load stretching surface
	gTexture = loadTexture("texture.png");
	if (gTexture == NULL)
	{
		printf("Failed to load texture image!\n");
		success = false;
	}
	return success;
}

void close()
{
	// �ͷ�������Դ
	SDL_DestroyTexture(gTexture);
	gTexture = NULL;

	// �ͷ���Ⱦ��
	SDL_DestroyRenderer(gRenderer);
	gRenderer = NULL;
	// �ͷŴ���
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;

	// �˳�SDL��ϵͳ
	IMG_Quit();
	SDL_Quit();
}

SDL_Texture* loadTexture(const string& path)
{
	SDL_Texture* newTexture = NULL;
	//SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	//if (loadedSurface == NULL)
	//{
	//	printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	//	
	//}
	//Create texture from surface pixels
	newTexture = IMG_LoadTexture(gRenderer,path.c_str());
	if (newTexture == NULL)
	{
		printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
	}
	//Get rid of old loaded surface
	//SDL_FreeSurface(loadedSurface);
	return newTexture;	
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

		//Clear screen
		SDL_RenderClear(gRenderer);
		//Render texture to screen
		SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);
		//Update screen
		SDL_RenderPresent(gRenderer);
	}
	close();
	return 0;
}