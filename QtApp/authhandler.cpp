#include "authhandler.h"
#include <QTimer>
#include <QDebug>
#include <QVariantMap>
#include <QNetworkRequest>
#include <QJsonObject>

AuthHandler::AuthHandler(QObject *parent)
    : QObject{parent}, m_apiKey(QString())
{
    m_networkAccessManager = new QNetworkAccessManager(this);
    connect(this, &AuthHandler::userSignedIn, this, &AuthHandler::performAuthenticatedDatabaseCall);
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(TimerSlot()));
    timer->start(1000);
    mainWindow = new MainWindow;
}

AuthHandler::~AuthHandler()
{
    m_networkAccessManager ->deleteLater();
}

void AuthHandler::setApiKey(const QString &apiKey)
{
    m_apiKey = apiKey;
}

void AuthHandler::signUserUp(const QString &emailAddress, const QString &password)
{
    QString signUpEndPoint = "https://identitytoolkit.googleapis.com/v1/accounts:signUp?key=" + m_apiKey;
    QVariantMap variantPayload;
    variantPayload["email"]=emailAddress;
    variantPayload["password"]=password;
    variantPayload["returnSecureToken"] = true;

    QJsonDocument jsonPayload = QJsonDocument::fromVariant(variantPayload);
    performPOST(signUpEndPoint, jsonPayload);
}

void AuthHandler::signUserIn(const QString &emailAddress, const QString &password)
{
    QString signInEndPoint = "https://identitytoolkit.googleapis.com/v1/accounts:signInWithPassword?key=" + m_apiKey;
    QVariantMap variantPayload;
    variantPayload["email"]=emailAddress;
    variantPayload["password"]=password;
    variantPayload["returnSecureToken"] = true;

    QJsonDocument jsonPayload = QJsonDocument::fromVariant(variantPayload);
    performPOST(signInEndPoint, jsonPayload);

}

void AuthHandler::setParametersOfMainWindow(QString led_status, QString led_color)
{
    if(led_status == "off" || led_status == "")
    {
        mainWindow -> setLedColor("grey");
        mainWindow -> setLedStatus("off");
    }
    else if (led_color == "" && led_status == "on")
    {
     mainWindow -> setLedColor("yellow");
     mainWindow -> setLedStatus(led_status);
    }
    else if (led_color == "" && led_status == "")
    {
     mainWindow -> setLedColor("grey");
     mainWindow -> setLedStatus("off");
    }
    else
    {
        mainWindow -> setLedColor(led_color);
        mainWindow -> setLedStatus(led_status);
    }
    mainWindow -> update();
}

void AuthHandler::networkReplyReadyRead()
{
    QByteArray response = m_networkReply->readAll();
    m_networkReply->deleteLater();
    parseResponse(response);
}

void AuthHandler::performAuthenticatedDatabaseCall()
{
    QString endPoint = "https://controlled-virtual-led-75cb0-default-rtdb.europe-west1.firebasedatabase.app/leds/1.json?auth=" + m_idToken;

    m_networkReply = m_networkAccessManager->get(QNetworkRequest(QUrl(endPoint)));
    connect(m_networkReply, &QNetworkReply::readyRead, this, &AuthHandler::networkReplyReadyRead);
}

void AuthHandler::performPOST(const QString &url, const QJsonDocument &payload)
{
    QNetworkRequest newRequest((QUrl(url)));
    newRequest.setHeader(QNetworkRequest::ContentTypeHeader, QString("application/json"));
    m_networkReply = m_networkAccessManager->post(newRequest, payload.toJson());
    connect(m_networkReply, &QNetworkReply::readyRead, this, &AuthHandler::networkReplyReadyRead);
}

void AuthHandler::TimerSlot(){
    performAuthenticatedDatabaseCall();
}

void AuthHandler::parseResponse(const QByteArray &response)
{
    QJsonDocument jsonDocument = QJsonDocument::fromJson(response);
    if(jsonDocument.object().contains("error")){
        qDebug() << "Error occured !" << response;
    }
    else if (jsonDocument.object().contains("kind")){
        QString idToken = jsonDocument.object().value("idToken").toString();
        qDebug() << "User signed in successfully !";
        m_idToken = idToken;
        emit userSignedIn();
    }
    else if (jsonDocument.object().contains("led_status")){
        QString led_status = jsonDocument.object().value("led_status").toString();
        QString led_color = jsonDocument.object().value("color").toString();
        setParametersOfMainWindow(led_status, led_color);
        mainWindow -> show();
        qDebug() << "state = " + led_status + " / color = " + led_color;
    }
    else
        qDebug() << "Request to firebase with user's token sucessfully ! There is the response : " + response;
}
