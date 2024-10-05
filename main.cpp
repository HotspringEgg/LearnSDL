#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include <string>
#include <cmath>
#include <iostream>

using namespace std;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

class LTexture
{
public:
	LTexture();
	~LTexture();
	bool loadFromFile(std::string path);
	void free();
	void setColor(Uint8 red, Uint8 green, Uint8 blue);
	void setBlendMode(SDL_BlendMode blending);
	void setAlpha(Uint8 alpha);
	void render(int x, int y, SDL_FRect* clip = NULL, double angle = 0.0, SDL_FPoint* center = NULL, SDL_FlipMode flip = SDL_FLIP_NONE);
	int getWidth();
	int getHeight();

private:
	SDL_Texture *mTexture;
	int mWidth;
	int mHeight;
};

bool init();
bool loadMedia();
void close();

SDL_Window *gWindow = NULL;
SDL_Renderer *gRenderer = NULL;

// Scene textures
LTexture gArrowTexture;	

LTexture::LTexture()
{
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}

LTexture::~LTexture()
{
	free();
}

bool LTexture::loadFromFile(std::string path)
{
	free();
	SDL_Texture *newTexture = NULL;
	SDL_Surface *loadedSurface = SDL_LoadBMP(path.c_str());

	if (loadedSurface == NULL)
	{
		SDL_Log("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), SDL_GetError());
		return false;
	}

	SDL_SetSurfaceColorKey(loadedSurface, true, SDL_MapSurfaceRGB(loadedSurface, 0, 0xFF, 0xFF));
	newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);

	if (newTexture == NULL)
	{
		SDL_Log("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		return false;
	}
	mWidth = loadedSurface->w;
	mHeight = loadedSurface->h;
	SDL_DestroySurface(loadedSurface);
	mTexture = newTexture;
	return mTexture != NULL;
}

void LTexture::free()
{
	if (mTexture != NULL)
	{
		SDL_DestroyTexture(mTexture);
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

void LTexture::setColor(Uint8 red, Uint8 green, Uint8 blue)
{
	SDL_SetTextureColorMod(mTexture, red, green, blue);
}

void LTexture::setBlendMode(SDL_BlendMode blending)
{
	SDL_SetTextureBlendMode(mTexture, blending);
}

void LTexture::setAlpha(Uint8 alpha)
{
	SDL_SetTextureAlphaMod(mTexture, alpha);
}

void LTexture::render(int x, int y, SDL_FRect *clip, double angle, SDL_FPoint* center, SDL_FlipMode flip)
{
	SDL_FRect renderQuad = {(float)x, (float)y, (float)mWidth, (float)mHeight};
	if (clip != NULL)
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}
	// 将纹理clip位置的图像渲染到窗口renderQuad位置
	SDL_RenderTextureRotated(gRenderer, mTexture, clip, &renderQuad, angle, center, flip);
}

int LTexture::getWidth()
{
	return mWidth;
}

int LTexture::getHeight()
{
	return mHeight;
}

bool init()
{
	if (!SDL_Init(SDL_INIT_VIDEO))
	{
		SDL_Log("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		return false;
	}

	// Create window and renderer
	if (!SDL_CreateWindowAndRenderer("SDL Tutorial", SCREEN_WIDTH, SCREEN_HEIGHT, 0, &gWindow, &gRenderer))
	{
		SDL_Log("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
		return false;
	}

	// enable vsync
	SDL_SetRenderVSync(gRenderer, 1);

	// Initialize renderer color
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
	return true;
}

bool loadMedia()
{
	// Load PNG texture
	if (!gArrowTexture.loadFromFile("arrow.bmp"))
	{
		SDL_Log("Failed to load sprite fadeout texture!\n");
		return false;
	}

	return true;
}

void close()
{
	// Free loaded image
	gArrowTexture.free();
	// Destroy window
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

	// Quit SDL subsystems
	SDL_Quit();
}

int main(int argc, char *args[])
{
	// Start up SDL and create window
	if (!init())
	{
		SDL_Log("Failed to initialize!\n");
		return -1;
	}

	// Load media
	if (!loadMedia())
	{
		SDL_Log("Failed to load media!\n");
		return -1;
	}

	// Main loop flag
	bool quit = false;
	// Event handler
	SDL_Event e;
	double degrees = 0;
	SDL_FlipMode flipType = SDL_FLIP_NONE;
	// While application is running
	while (!quit)
	{
		// Handle events on queue
		while (SDL_PollEvent(&e) != 0)
		{
			// User requests quit
			if (e.type == SDL_EVENT_QUIT)
			{
				quit = true;
			}
			else if (e.type == SDL_EVENT_KEY_DOWN)
			{
				if (e.key.key == SDLK_ESCAPE)
				{
					quit = true;
				}
				else if (e.key.key == SDLK_A)
				{
					degrees -= 60;
					cout << "degrees: " << degrees << endl;
				}
				else if (e.key.key == SDLK_D)
				{
					degrees += 60;
				}
				else if (e.key.key == SDLK_Q)
				{
					flipType = SDL_FLIP_HORIZONTAL;
				}
				else if (e.key.key == SDLK_W)
				{
					flipType = SDL_FLIP_NONE;
				}
				else if (e.key.key == SDLK_E)
				{
					flipType = SDL_FLIP_VERTICAL;
				}
			}
		}
		SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(gRenderer);

		gArrowTexture.render((SCREEN_WIDTH - gArrowTexture.getWidth()) / 2, (SCREEN_HEIGHT - gArrowTexture.getHeight()) / 2, NULL, degrees, NULL, flipType);
		
		// 更新屏幕
		SDL_RenderPresent(gRenderer);
	}
	close();
	return 0;
}
