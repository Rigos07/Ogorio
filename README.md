# Ogorio
Projet IR 2A - bot Ogar de type agar.io

Trinome n°2
Membres : Yann Valentin Lucas

# Bots
SOLO contient les algorithmes nécessaires ainsi qu'un executable qui permet d'envoyer 6 dogs ramener toutes les brebis dans l'enclos

PVP contient lui 2 branches, une pour chaque côté, qui permet d'affronter une autre équipe de chiens.

# Principe
Mode solo :
L'algorithme utilise :
- 1 chien Green qui nettoie une petite zone indépendamment des autres, circule sur un circuit secondaire
- 4 chiens Yellow qui ramène les brebis qu'ils rencontre et circulent sur le circuit principal si ils n'en trouvent pas afin de recueillir des informations venant de Blue
- 1 chien Blue qui circule sur le circuit principal et communique la positions des brebis aux Yellow via le protocole Joystick ©

Mode duel : 
Composition de l'équipe :
- 1 chien Green qui nettoie une petite zone indépendamment des autres, circule sur un circuit secondaire et protège la bergerie des éventuelles attaques
- 3 chiens Yellow qui ramène les brebis qu'ils rencontre et circulent sur le circuit principal si ils n'en trouvent pas afin de recueillir des informations venant de Blue
- 1 chien Blue qui circule sur le circuit principal et communique la positions des brebis aux Yellow via le protocole Joystick ©
- 1 chien Cyan qui va se rendre dans la bergerie ennemie afin d'en faire sortir les brebis

# Mentions spéciales

Le Protocole Joystick © est un nom déposé, caractéristique propre à ce groupe.
