#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include "lombric.h"
#include "controle.h"

Direction EntreeJoueur(void)
{
	SDL_Event event;		
		if (SDL_PollEvent(&event)) 
		{
			switch(event.type)
			{
				case SDL_KEYDOWN:
					switch(event.key.keysym.sym)
					{
						case SDLK_DOWN:
							return BAS;
							break;
						case SDLK_UP:
							return HAUT;
							break;
						case SDLK_LEFT:
							return GAUCHE;
							break;
						case SDLK_RIGHT:
							return DROITE;
							break;
						case SDLK_RETURN:
							return COMMENCER;
							break;
						case SDLK_SPACE:
							return PAUSE;
							break;
						case SDLK_ESCAPE:
							return FERMERFENETRE;
							break;
						default:
							break;
					}
					break;
				case SDL_KEYUP:
					switch(event.key.keysym.sym)
					{
						case SDLK_DOWN:
							return SANS;
							break;
						case SDLK_UP:
							return SANS;
							break;
						case SDLK_LEFT:
							return SANS;
							break;
						case SDLK_RIGHT:
							return SANS;
							break;						
						default:
							break;
					}
					break;
				case SDL_JOYBUTTONDOWN:
					switch(event.jbutton.button)
					{
						case 7:
							return COMMENCER;
							break;
						case 0:
							return PAUSE;
							break;
						case 6:
							return PAUSE;
							break;
						default:				
							break;
					}
					break;
				case SDL_JOYAXISMOTION:
					if((event.jaxis.value < -8000) || (event.jaxis.value > 8000))
					{
						if(event.jaxis.axis == 0)
						{
							if(event.jaxis.value < 0)
								return GAUCHE;
							else if(event.jaxis.value > 0)
								return DROITE;
						}
						if(event.jaxis.axis == 1)
						{
							if(event.jaxis.value < 1)
								return HAUT;
							else if(event.jaxis.value > 1)
								return BAS;
						}
					}
					break;				
				case SDL_QUIT: 
					return FERMERFENETRE;
					break;			
				default:
					break;
			}
		}
		return SANS;
}
