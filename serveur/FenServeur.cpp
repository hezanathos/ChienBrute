//http://piratepad.net/chienbrute
#include "FenServeur.h"
#include "perso.h"
#include <QDebug>

FenServeur::FenServeur()
{
    // Création et disposition des widgets de la fenêtre
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
    if (!serveur->listen(QHostAddress::Any, 58885)) // Démarrage du serveur sur toutes les IP disponibles et sur le port 50585
    {
        // Si le serveur n'a pas été démarré correctement
        etatServeur->setText(tr("Le serveur n'a pas pu être démarré. Raison :<br />") + serveur->errorString());
    }
    else
    {
        // Si le serveur a été démarré correctement
        etatServeur->setText(tr("Le serveur a été démarré sur le port <strong>") + QString::number(serveur->serverPort()) + tr("</strong>.<br />Des clients peuvent maintenant se connecter."));
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
    // 1 : on reçoit un paquet (ou un sous-paquet) d'un des clients

    // On détermine quel client envoie le message (recherche du QTcpSocket du client)
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    if (socket == 0) // Si par hasard on n'a pas trouvé le client à l'origine du signal, on arrête la méthode
        return;

    // Si tout va bien, on continue : on récupère le message
    QDataStream in(socket);

    if (tailleMessage == 0) // Si on ne connaît pas encore la taille du message, on essaie de la récupérer
    {
        if (socket->bytesAvailable() < (int)sizeof(quint16)) // On n'a pas reçu la taille du message en entier
             return;

        in >> tailleMessage; // Si on a reçu la taille du message en entier, on la récupère
    }

    // Si on connaît la taille du message, on vérifie si on a reçu le message en entier
    if (socket->bytesAvailable() < tailleMessage) // Si on n'a pas encore tout reçu, on arrête la méthode
        return;


    // Si ces lignes s'exécutent, c'est qu'on a reçu tout le message : on peut le récupérer !
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
    // 2 : on renvoie le message à tous les clients
    envoyerATous(message);

}
    tailleMessage = 0;
    // 3 : remise de la taille du message à 0 pour permettre la réception des futurs messages

}

void FenServeur::deconnexionClient()
{
    envoyerATous(tr("<em>Un client vient de se déconnecter</em>"));

    // On détermine quel client se déconnecte
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    if (socket == 0) // Si par hasard on n'a pas trouvé le client à l'origine du signal, on arrête la méthode
        return;

    clients.removeOne(socket);

    socket->deleteLater();
}

void FenServeur::envoyerATous(const QString &message)
{
    // Préparation du paquet
    QByteArray paquet;
    QDataStream out(&paquet, QIODevice::WriteOnly);

    out << (quint16) 0; // On écrit 0 au début du paquet pour réserver la place pour écrire la taille
    out << message; // On ajoute le message à la suite
    out.device()->seek(0); // On se replace au début du paquet
    out << (quint16) (paquet.size() - sizeof(quint16)); // On écrase le 0 qu'on avait réservé par la longueur du message


    // Envoi du paquet préparé à tous les clients connectés au serveur
    for (int i = 0; i < clients.size(); i++)
    {
        clients[i]->write(paquet);
    }
 tailleMessage = 0;
}
void FenServeur::envoyerAquelqun(QTcpSocket *destinataire, const QString &message)
{
	// Préparation du paquet
	QByteArray paquet;
	QDataStream out(&paquet, QIODevice::WriteOnly);

	out << (quint16)0; // On écrit 0 au début du paquet pour réserver la place pour écrire la taille
	out << message; // On ajoute le message à la suite
	out.device()->seek(0); // On se replace au début du paquet
	out << (quint16)(paquet.size() - sizeof(quint16)); // On écrase le 0 qu'on avait réservé par la longueur du message


    // Envoi du paquet préparé au bon clients connectés au serveur

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
retour +="<em>Voici les caractéristiques que dame nature a bien voulues t'accorder : </em>";
retour+="<br>";
retour+=QString("<em>intelligence : %1</em>").arg(first.getIntelligence());
retour+="<br>";
retour+=QString("<em>vitalité : %1</em>").arg(first.getVitalite());
retour+="<br>";
retour+=QString("<em>Agilité : %1</em>").arg(first.getAgilite());
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
    retour = "Tu t'es tapé tout seul. Vraiement très con.";
    retour+="<br>";
}
int i = 0;
retour = QString("Début du combat");
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
         retour+="<em>Initialisation terminée";
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




