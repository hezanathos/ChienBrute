#include "Perso.h"
#include <stdlib.h>
#include <time.h>
Perso::Perso(QString nom)
{   qDebug() << "constr";
    int total = 0;
    srand(time(NULL));

    vitalite = rand() % 100 + 1;
    total += vitalite;
    agilite = rand() % 100 + 1;
    total += agilite;
    force = rand() % 100 + 1;
    total += force;
    intelligence = rand() % 100 + 1;
    total += intelligence;
    qDebug() << intelligence;
    agilite = agilite * 100 / total ;
    force = force * 100 / total ;
    intelligence = intelligence * 100 / total ;
    vitalite = vitalite * 100 / total ;

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
