/*
 * Copyright (c) 2018, 2019 Amine Ben Hassouna <amine.benhassouna@gmail.com>
 * All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any
 * person obtaining a copy of this software and associated
 * documentation files (the "Software"), to deal in the
 * Software without restriction, including without
 * limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software
 * is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice
 * shall be included in all copies or substantial portions
 * of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF
 * ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
 * TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT
 * SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR
 * IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 */

#include <stdio.h>
#include <stdbool.h>

#include <SDL.h>
#include <SDL_mixer.h>

// Define MAX and MIN macros
#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))
#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))

// Define screen dimensions
#define SCREEN_WIDTH    800
#define SCREEN_HEIGHT   600

#define WAVES_SOUND "assets/waves-at-baltic-sea-shore/waves-at-baltic-sea-shore.wav"

#define CLAP_SOUND              "assets/claps-and-snares/clap.ogg"
#define SNARE_SOUND             "assets/claps-and-snares/snare.ogg"
#define TECHNO_CLAP_SNARE_SOUND "assets/claps-and-snares/techno-clap-snare.ogg"
#define REVERB_SNARE_SOUND      "assets/claps-and-snares/dubstep-reverb-snare.ogg"

int main(int argc, char* argv[])
{
    // Unused argc, argv
    (void) argc;
    (void) argv;

    // Initialize SDL
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
    {
        printf("SDL could not be initialized!\n"
               "SDL_Error: %s\n", SDL_GetError());
        return 0;
    }

    //Initialize SDL2_mixer
    if(Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
    {
        printf("SDL2_mixer could not be initialized!\n"
               "SDL_Error: %s\n", SDL_GetError());
        return 0;
    }

#if defined linux && SDL_VERSION_ATLEAST(2, 0, 8)
    // Disable compositor bypass
    if(!SDL_SetHint(SDL_HINT_VIDEO_X11_NET_WM_BYPASS_COMPOSITOR, "0"))
    {
        printf("SDL can not disable compositor bypass!\n");
        return 0;
    }
#endif

    // Create window
    SDL_Window *window = SDL_CreateWindow("SDL2 audio sample (Press SPACE to pause/play)",
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SCREEN_WIDTH, SCREEN_HEIGHT,
                                          SDL_WINDOW_SHOWN);
    if(!window)
    {
        printf("Window could not be created!\n"
               "SDL_Error: %s\n", SDL_GetError());
    }
    else
    {
        // Create renderer
        SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if(!renderer)
        {
            printf("Renderer could not be created!\n"
                   "SDL_Error: %s\n", SDL_GetError());
        }
        else
        {
            // Load .WAV sound
            Mix_Chunk *waves = Mix_LoadWAV(WAVES_SOUND);
            if(!waves)
            {
                printf(".WAV sound '%s' could not be loaded!\n"
                       "SDL_Error: %s\n", WAVES_SOUND, SDL_GetError());
                return 0;
            }

            // Load .OGG sounds
            Mix_Music *clapnsnare1 = Mix_LoadMUS(CLAP_SOUND);
            Mix_Music *clapnsnare2 = Mix_LoadMUS(SNARE_SOUND);
            Mix_Music *clapnsnare3 = Mix_LoadMUS(TECHNO_CLAP_SNARE_SOUND);
            Mix_Music *clapnsnare4 = Mix_LoadMUS(REVERB_SNARE_SOUND);
            if(!clapnsnare1 || !clapnsnare2 || !clapnsnare3 || !clapnsnare4)
            {
                printf("One of the .OGG sounds could not be loaded!\n"
                       "SDL_Error: %s\n", SDL_GetError());
                return 0;
            }

            // Play waves sound
            if(Mix_PlayChannel(-1, waves, 0) == -1)
            {
                printf("Waves sound could not be played!\n"
                       "SDL_Error: %s\n", SDL_GetError());
                Mix_FreeChunk(waves);
                return 0;
            }

            // Declare rect of square
            SDL_Rect squareRect;

            // Square dimensions: Half of the min(SCREEN_WIDTH, SCREEN_HEIGHT)
            squareRect.w = MIN(SCREEN_WIDTH, SCREEN_HEIGHT) / 2;
            squareRect.h = MIN(SCREEN_WIDTH, SCREEN_HEIGHT) / 2;

            // Square position: In the middle of the screen
            squareRect.x = SCREEN_WIDTH / 2 - squareRect.w / 2;
            squareRect.y = SCREEN_HEIGHT / 2 - squareRect.h / 2;


            // Declare rects of pause symbol
            SDL_Rect pauseRect1, pauseRect2;
            pauseRect1.h = squareRect.h / 2;
            pauseRect1.w = 40;
            pauseRect1.x = squareRect.x + (squareRect.w - pauseRect1.w*3) / 2;
            pauseRect1.y = squareRect.y + squareRect.h / 4;
            pauseRect2 = pauseRect1;
            pauseRect2.x += pauseRect1.w * 2;


            // Event loop exit flag
            bool quit = false;

            // Event loop
            while(!quit)
            {
                SDL_Event e;

                // Wait indefinitely for the next available event
                SDL_WaitEvent(&e);

                // User requests quit
                if(e.type == SDL_QUIT)
                {
                    quit = true;
                }
                else if(e.type == SDL_KEYDOWN)
                {
                    switch (e.key.keysym.sym)
                    {
                    case SDLK_SPACE:
                        if(Mix_Paused(-1))
                            Mix_Resume(-1);
                        else
                            Mix_Pause(-1);
                        break;

                    case SDLK_RIGHT:
                        if(Mix_PlayMusic(clapnsnare1, 1) == -1)
                        {
                            printf(".OGG sound could not be played!\n"
                                   "SDL_Error: %s\n", SDL_GetError());
                        }
                        break;

                    case SDLK_LEFT:
                        if(Mix_PlayMusic(clapnsnare2, 1) == -1)
                        {
                            printf(".OGG sound could not be played!\n"
                                   "SDL_Error: %s\n", SDL_GetError());
                        }
                        break;

                    case SDLK_UP:
                        if(Mix_PlayMusic(clapnsnare3, 1) == -1)
                        {
                            printf(".OGG sound could not be played!\n"
                                   "SDL_Error: %s\n", SDL_GetError());
                        }
                        break;

                    case SDLK_DOWN:
                        if(Mix_PlayMusic(clapnsnare4, 1) == -1)
                        {
                            printf(".OGG sound could not be played!\n"
                                   "SDL_Error: %s\n", SDL_GetError());
                        }
                        break;
                    }
                }

                // Initialize renderer color white for the background
                SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

                // Clear screen
                SDL_RenderClear(renderer);

                // Set renderer color blue to draw the square
                SDL_SetRenderDrawColor(renderer, 0x19, 0x71, 0xA9, 0xFF);

                // Draw filled square
                SDL_RenderFillRect(renderer, &squareRect);

                // Check pause status
                if(Mix_Paused(-1))
                {
                    // Set renderer color white to draw the pause symbol
                    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

                    // Draw pause symbol
                    SDL_RenderFillRect(renderer, &pauseRect1);
                    SDL_RenderFillRect(renderer, &pauseRect2);
                }


                // Update screen
                SDL_RenderPresent(renderer);
            }

            // Clean up audio
            Mix_FreeChunk(waves);
            Mix_FreeMusic(clapnsnare1);
            Mix_FreeMusic(clapnsnare2);
            Mix_FreeMusic(clapnsnare3);
            Mix_FreeMusic(clapnsnare4);

            // Destroy renderer
            SDL_DestroyRenderer(renderer);
        }

        // Destroy window
        SDL_DestroyWindow(window);
    }

    // Quit SDL2_mixer
    Mix_CloseAudio();

    // Quit SDL
    SDL_Quit();

    return 0;
}
