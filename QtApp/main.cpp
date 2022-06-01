#include <QApplication>
#include "authhandler.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    AuthHandler authHandler;
    authHandler.setApiKey("<Paste your Firebase API Key Web app here>");

    //For register a new user use this :
    //authHandler.signUserUp("<New Email>", "<New strongly password>");

    //It's not recommended to paste your credentials directly in the code, use environnement variables instead or an other way !
    authHandler.signUserIn("<Email of your registred custom user in Firebase>", "<Strongly password of your registred custom user in Firebase>");
    return a.exec();
}
