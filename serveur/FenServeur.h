#ifndef HEADER_FENSERVEUR
#define HEADER_FENSERVEUR

#include <QtWidgets>
#include <QtNetwork>
#include "perso.h"

class FenServeur : public QWidget
{
    Q_OBJECT

    public:
        FenServeur();
        void envoyerATous(const QString &message);
        void envoyerAquelqun(QTcpSocket *destinataire, const QString &message);
        QString dollarInit(QStringList list);
        QString dollarCombat(QStringList list);
         QString controlleurDeJeu(QStringList list);


    private slots:
        void nouvelleConnexion();
        void donneesRecues();
        void deconnexionClient();

    private:
        QLabel *etatServeur;
        QPushButton *boutonQuitter;

        QTcpServer *serveur;
        QList<QTcpSocket *> clients;
        QList<Perso> persos;
        quint16 tailleMessage;

};

#endif
