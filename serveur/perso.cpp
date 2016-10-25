#include "Perso.h"
#include <stdlib.h>
#include <time.h>
Perso::Perso(QString nom)
{
    int total = 100;
    srand(time(NULL));

    vitalite = rand() % 100 + 1;
    total -= vitalite;
    agilite = rand() % total + 1;
    total -= agilite;
    force = rand() % total + 1;
    total -= force;
    intelligence = total;
    pseudo = nom;
}

void Perso::setVitalite(int vitalite){this->vitalite=vitalite;}
void Perso::setAgilite(int agilite){this->agilite=agilite;}
void Perso::setForce(int force){this->force=force;}
void Perso::setIntelligence (int intelligence){this->intelligence=intelligence;}
void Perso::setPseudo(QString pseudo){this->pseudo=pseudo;}

int Perso::getVitalite(){return vitalite;}
int Perso::getAgilite(){return agilite;}
int Perso::getForce(){return force;}
int Perso::getIntelligence (){return intelligence;}
QString Perso::getPseudo(){return pseudo;}
