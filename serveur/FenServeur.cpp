//http://piratepad.net/chienbrute
#include "FenServeur.h"
#include "perso.h"
#include <QDebug>

FenServeur::FenServeur()
{
    // Cr�ation et disposition des widgets de la fen�tre
    etatServeur = new QLabel;
    boutonQuitter = new QPushButton(tr("Quitter"));
    connect(boutonQuitter, SIGNAL(clicked()), qApp, SLOT(quit()));

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(etatServeur);
    layout->addWidget(boutonQuitter);
    setLayout(layout);

    setWindowTitle(tr("ChienBrute - Serveur"));

    // Gestion du serveur
    serveur = new QTcpServer(this);
    if (!serveur->listen(QHostAddress::Any, 58885)) // D�marrage du serveur sur toutes les IP disponibles et sur le port 50585
    {
        // Si le serveur n'a pas �t� d�marr� correctement
        etatServeur->setText(tr("Le serveur n'a pas pu �tre d�marr�. Raison :<br />") + serveur->errorString());
    }
    else
    {
        // Si le serveur a �t� d�marr� correctement
        etatServeur->setText(tr("Le serveur a �t� d�marr� sur le port <strong>") + QString::number(serveur->serverPort()) + tr("</strong>.<br />Des clients peuvent maintenant se connecter."));
        connect(serveur, SIGNAL(newConnection()), this, SLOT(nouvelleConnexion()));
    }

    tailleMessage = 0;
}

void FenServeur::nouvelleConnexion()
{
    envoyerATous(tr("<em>Un nouveau client vient de se connecter</em>"));

    QTcpSocket *nouveauClient = serveur->nextPendingConnection();
    clients << nouveauClient;

    connect(nouveauClient, SIGNAL(readyRead()), this, SLOT(donneesRecues()));
    connect(nouveauClient, SIGNAL(disconnected()), this, SLOT(deconnexionClient()));
}

void FenServeur::donneesRecues()
{
    // 1 : on re�oit un paquet (ou un sous-paquet) d'un des clients

    // On d�termine quel client envoie le message (recherche du QTcpSocket du client)
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    if (socket == 0) // Si par hasard on n'a pas trouv� le client � l'origine du signal, on arr�te la m�thode
        return;

    // Si tout va bien, on continue : on r�cup�re le message
    QDataStream in(socket);

    if (tailleMessage == 0) // Si on ne conna�t pas encore la taille du message, on essaie de la r�cup�rer
    {
        if (socket->bytesAvailable() < (int)sizeof(quint16)) // On n'a pas re�u la taille du message en entier
             return;

        in >> tailleMessage; // Si on a re�u la taille du message en entier, on la r�cup�re
    }

    // Si on conna�t la taille du message, on v�rifie si on a re�u le message en entier
    if (socket->bytesAvailable() < tailleMessage) // Si on n'a pas encore tout re�u, on arr�te la m�thode
        return;


    // Si ces lignes s'ex�cutent, c'est qu'on a re�u tout le message : on peut le r�cup�rer !
    QString message;
	std::string messageString = message.toStdString();
    in >> message;

    QStringList list = message.split(" ");
    QString consigne = list.at(2);

    QChar firstChar = consigne.at(0);

    if (firstChar == QChar('$')){
     QString retour =  FenServeur::controlleurDeJeu(list);


            FenServeur::envoyerAquelqun(socket,retour);



        }









else
{
    // 2 : on renvoie le message � tous les clients
    envoyerATous(message);

}
    tailleMessage = 0;
    // 3 : remise de la taille du message � 0 pour permettre la r�ception des futurs messages

}

void FenServeur::deconnexionClient()
{
    envoyerATous(tr("<em>Un client vient de se d�connecter</em>"));

    // On d�termine quel client se d�connecte
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    if (socket == 0) // Si par hasard on n'a pas trouv� le client � l'origine du signal, on arr�te la m�thode
        return;

    clients.removeOne(socket);

    socket->deleteLater();
}

void FenServeur::envoyerATous(const QString &message)
{
    // Pr�paration du paquet
    QByteArray paquet;
    QDataStream out(&paquet, QIODevice::WriteOnly);

    out << (quint16) 0; // On �crit 0 au d�but du paquet pour r�server la place pour �crire la taille
    out << message; // On ajoute le message � la suite
    out.device()->seek(0); // On se replace au d�but du paquet
    out << (quint16) (paquet.size() - sizeof(quint16)); // On �crase le 0 qu'on avait r�serv� par la longueur du message


    // Envoi du paquet pr�par� � tous les clients connect�s au serveur
    for (int i = 0; i < clients.size(); i++)
    {
        clients[i]->write(paquet);
    }
 tailleMessage = 0;
}
void FenServeur::envoyerAquelqun(QTcpSocket *destinataire, const QString &message)
{
	// Pr�paration du paquet
	QByteArray paquet;
	QDataStream out(&paquet, QIODevice::WriteOnly);

	out << (quint16)0; // On �crit 0 au d�but du paquet pour r�server la place pour �crire la taille
	out << message; // On ajoute le message � la suite
	out.device()->seek(0); // On se replace au d�but du paquet
	out << (quint16)(paquet.size() - sizeof(quint16)); // On �crase le 0 qu'on avait r�serv� par la longueur du message


    // Envoi du paquet pr�par� au bon clients connect�s au serveur

		destinataire->write(paquet);


}
 QString FenServeur::dollarInit(QStringList list)
{
QString retour;

for (int i = 0; i<persos.size();i++){


    if (persos.at(i).getPseudo() == list.at(0)) {
      retour = "un seul personnage par compte jeune sacripan ( ou bien vil faquin )!" ;
       return retour;
    }
}
Perso first(list.at(0));
persos << first;
retour =QString("<em>Bienvenue au monde jeune "+first.getPseudo()+"</em>");
retour+="<br>";
retour +="<em>Voici les caract�ristiques que dame nature a bien voulues t'accorder : </em>";
retour+="<br>";
retour+=QString("<em>intelligence : %1</em>").arg(first.getIntelligence());
retour+="<br>";
retour+=QString("<em>vitalit� : %1</em>").arg(first.getVitalite());
retour+="<br>";
retour+=QString("<em>Agilit� : %1</em>").arg(first.getAgilite());
retour+="<br>";
retour+=QString("<em>Force : %1</em>").arg(first.getForce());
retour+="<br>";
retour +="<em>Il semblerait qu'elle soit dans un mauvais jour !</em>";
retour+="<br>";
retour +="<em>Faudra faire avec... </em>";
retour+="<br>";
qDebug() << retour;
return retour;
}

 QString FenServeur::dollarCombat(QStringList list)
{
 Perso *perso1;
 Perso *perso2;
QString retour;
QString param = "<strong>"+list.at(3)+"</strong>";
for (int i = 0; i<persos.size();i++){


    if (persos.at(i).getPseudo() == param) {
        perso2 = &persos[i];

    }
    if (persos.at(i).getPseudo() == list.at(0)) {
        perso1 = &persos[i]  ;

    }
}

if ( perso1->getPseudo() == perso2->getPseudo()){

    perso1->setPv(perso1->getPv()-500);
    retour = "Tu t'es tap� tout seul. Vraiement tr�s con.";
    retour+="<br>";
}
int i = 0;
retour = QString("D�but du combat");
retour+="<br>";
while (perso1->getPv()>0 && perso2->getPv()>0)
{ retour += QString("round ") + i + QString("vie de ") + perso1->getPseudo() + " = " + perso1->getPv();
retour += QString(" vie de ") + perso2->getPseudo() + " = " + perso2->getPv();
retour+="<br>";
perso1->setPv(perso1->getPv()- perso2->getForce());
perso2->setPv(perso1->getPv()- perso1->getForce());



}

retour += QString("Fin du combat");
retour+="<br>";
return retour;
 }






QString FenServeur::controlleurDeJeu(QStringList list)
{

    QString retour;
    qDebug() << "Controlleur de jeu";
    if (list.at(2)== QString("$init")){
          retour = "<em>Initialisation en cours</em><br>";
         retour+= FenServeur::dollarInit(list);
         retour+="<br>";
         retour+="<em>Initialisation termin�e";
    }
    else if (list.at(2)== QString("$combat")){
        if (list.size() > 3) {

QString param = "<strong>"+list.at(3)+"</strong>";
            for (int i = 0; i<persos.size();i++){


                if (persos.at(i).getPseudo() == param) {
                    retour = "<em>Combat en cours</em><br>";
                  retour+= FenServeur::dollarCombat(list);

                   return retour;
                }
            }



        retour = "<em>Entrez un pseudo inscrit au grand livre des Ages !</em><br>";









        }else retour =QString("Entrez un adversaire");

    }
    else retour =QString("commande inconnue :")+list.at(2);

    return retour;
}




