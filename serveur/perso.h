#ifndef DEF_PERSONNAGE
#define DEF_PERSONNAGE
#include <QtWidgets>
#include <QtNetwork>
class Perso
{
private :
    int vitalite;
    int agilite;
    int force;
    int intelligence;
    QString pseudo;


public:
    Perso(QString);
   void setVitalite(int);
   void setAgilite(int);
   void setForce(int);
   void setIntelligence(int);
   void setPseudo(QString);

   int getVitalite();
   int getAgilite();
   int getForce();
   int getIntelligence ();
   QString getPseudo();
};
#endif
