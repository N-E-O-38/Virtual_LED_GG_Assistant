#ifndef AUTHHANDLER_H
#define AUTHHANDLER_H
#include <QTimer>
#include <QObject>
#include <qnetworkaccessmanager.h>
#include <QNetworkReply>
#include <QJsonDocument>
#include "mainwindow.h"

class AuthHandler : public QObject
{
    Q_OBJECT
public:
    explicit AuthHandler(QObject *parent = nullptr);
    ~AuthHandler();
    void setApiKey(const QString & apiKey);
    void signUserUp(const QString & emailAddress, const QString & password);
    void signUserIn(const QString & emailAddress, const QString & password);
    void setParametersOfMainWindow(QString led_status, QString led_color);
    QTimer *timer;
    MainWindow *mainWindow;

public slots:
    void networkReplyReadyRead();
    void performAuthenticatedDatabaseCall();
    void TimerSlot();
signals:
    void userSignedIn();
private:
    void performPOST(const QString & url, const QJsonDocument & payload);
    void parseResponse(const QByteArray & response);
    QString m_apiKey;
    QNetworkAccessManager * m_networkAccessManager;
    QNetworkReply * m_networkReply;
    QString m_idToken;
};

#endif // AUTHHANDLER_H
