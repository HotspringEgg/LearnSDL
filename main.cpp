#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include <string>
#include <cmath>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

class LTexture
{
public:
	LTexture();
	~LTexture();
	bool loadFromFile(std::string path);
	void free();
	void render(int x, int y, SDL_FRect *clip = NULL);
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
SDL_FRect gSpriteClips[4];
LTexture gSpriteSheetTexture;

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

void LTexture::render(int x, int y, SDL_FRect *clip)
{
	SDL_FRect renderQuad = {(float)x, (float)y, (float)mWidth, (float)mHeight};
	if (clip != NULL)
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}
	// 将纹理clip位置的图像渲染到窗口renderQuad位置
	SDL_RenderTexture(gRenderer, mTexture, clip, &renderQuad);
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

	// Initialize renderer color
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
	return true;
}

bool loadMedia()
{
	// Load PNG texture
	if (!gSpriteSheetTexture.loadFromFile("dots.bmp"))
	{
		SDL_Log("Failed to load sprite sheet texture!\n");
		return false;
	}

	// Set top left sprite
	gSpriteClips[0].x = 0;
	gSpriteClips[0].y = 0;
	gSpriteClips[0].w = 100;
	gSpriteClips[0].h = 100;

	// Set top right sprite
	gSpriteClips[1].x = 100;
	gSpriteClips[1].y = 0;
	gSpriteClips[1].w = 100;
	gSpriteClips[1].h = 100;

	// Set bottom left sprite
	gSpriteClips[2].x = 0;
	gSpriteClips[2].y = 100;
	gSpriteClips[2].w = 100;
	gSpriteClips[2].h = 100;

	// Set bottom right sprite
	gSpriteClips[3].x = 100;
	gSpriteClips[3].y = 100;
	gSpriteClips[3].w = 100;
	gSpriteClips[3].h = 100;
	return true;
}

void close()
{
	// Free loaded image
	gSpriteSheetTexture.free();

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
		}
		SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(gRenderer);

		gSpriteSheetTexture.render(0, 0, &gSpriteClips[0]);
		gSpriteSheetTexture.render(SCREEN_WIDTH - gSpriteClips[1].w, 0, &gSpriteClips[1]);
		gSpriteSheetTexture.render(0, SCREEN_HEIGHT - gSpriteClips[2].h, &gSpriteClips[2]);
		gSpriteSheetTexture.render(SCREEN_WIDTH - gSpriteClips[3].w, SCREEN_HEIGHT - gSpriteClips[3].h, &gSpriteClips[3]);
		// 更新屏幕
		SDL_RenderPresent(gRenderer);
	}
	close();
	return 0;
}
