## Lombric in C

par Sébastien Abilla

MIT License (voir LICENSE)

### C'est quoi?

Mon deuxième jeu. Pas très beau, mais fonctionnel?  
C'est un Snake-like game (original: PAS ENCORE) développé en C avec la SDL.  
Dans "Lombric in C", il faut PAS ENCORE.  
Jouable au clavier ou à la manette A CONFIRMER. 

Un aperçu de la "jouabilité" en vidéo PAS ENCORE  

![Aperçu](res/demo.png) A CONFIRMER

### Compilation

Développé dans l'éditeur Geany 1.37.1 et testé sous Xubuntu 21.10 64 bits.  

Pour pouvoir compiler, il faut avoir installé: A CONFIRMER

```bash
libsdl2-dev
libsdl2-mixer-dev
libsdl2-ttf-dev
```

puis entrer dans un terminal: A CONFIRMER

```bash
gcc *.c -o lombric -lm -lSDL2_ttf -lSDL2_mixer $(sdl2-config --cflags --libs)
```

### Crédits

De nombreuses parties du code sont adaptées ou inspirées de:
* CS50x by David J. Malan https://cs50.harvard.edu/x/2021
* FormationVideo - C cours by Jason Champagne https://www.youtube.com/c/Formationvidéo8/

Les sons sont extraits et/ou adaptés de:
* PAS ENCORE

Les sprites sont adaptés de:
* PAS ENCORE



