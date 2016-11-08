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
    int pv;
    QString pseudo;


public:
    Perso(QString);
   void setVitalite(int);
   void setAgilite(int);
   void setForce(int);
   void setIntelligence(int);
   void setPseudo(QString);
   void setPv(int);

   int getVitalite()const;
   int getAgilite()const;
   int getForce()const;
   int getIntelligence ()const;
   int getPv () const;
   QString getPseudo() const;
};
#endif
