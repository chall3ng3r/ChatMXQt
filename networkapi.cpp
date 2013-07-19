#include "networkapi.h"

NetworkApi::NetworkApi(QObject *parent) : QObject(parent)
{
    // init net manager
    nam = new QNetworkAccessManager(parent);

    // new line
    QObject::connect(nam, SIGNAL(finished(QNetworkReply*)), this, SLOT(handleNetworkReply(QNetworkReply*)));
}

void NetworkApi::loginUser(const QString *nick)
{
    QNetworkRequest request(QUrl("http://orison.biz/applications/chatwebservice.asmx/Login"));

    QByteArray data;
    QUrl params;

    params.addQueryItem("Nick", *nick);
    data.append(params.toEncoded());
    data.remove(0, 1);

    nam->post(request, data);
}

void NetworkApi::logoutUser(const QString *nick)
{
    QNetworkRequest request(QUrl("http://orison.biz/applications/chatwebservice.asmx/Logout"));

    QByteArray data;
    QUrl params;

    params.addQueryItem("Nick", *nick);
    data.append(params.toEncoded());
    data.remove(0, 1);

    nam->post(request, data);
}

void NetworkApi::getMessages(const QString *lastTime)
{
    QNetworkRequest request(QUrl("http://orison.biz/applications/chatwebservice.asmx/getMessages"));

    QByteArray data;
    QUrl params;

    params.addQueryItem("lastMesgTime", *lastTime);
    data.append(params.toEncoded());
    data.remove(0, 1);

    nam->post(request, data);
}

void NetworkApi::getUsers()
{
    QNetworkRequest request(QUrl("http://orison.biz/applications/chatwebservice.asmx/getUsers"));

    QByteArray data;

    nam->post(request, data);
}

void NetworkApi::postMessage(QString *nick, QString *message)
{
    QNetworkRequest request(QUrl("http://orison.biz/applications/chatwebservice.asmx/postMessage"));

    QByteArray data;
    QUrl params;

    params.addQueryItem("Nick", *nick);
    params.addQueryItem("Mesg", *message);
    data.append(params.toEncoded());
    data.remove(0, 1);

    nam->post(request, data);

    QString str;
    emit messagePosted(&str);
}

void NetworkApi::handleNetworkReply(QNetworkReply* reply)
{
    //QVariant statusCodeV = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);

    // no error received?
    if (reply->error() == QNetworkReply::NoError)
    {
        QString stringXML(reply->readAll()); // raw xml

        qDebug(QString("\n\n---------------\n").append(stringXML).toUtf8());

        // process data
        if(stringXML.contains("status_code=\"101\"", Qt::CaseInsensitive))
        {
            emit commError(&QString("Nick already taken"));
        }
        else if(stringXML.contains("<Users", Qt::CaseInsensitive))
        {
            emit usersReceived(&stringXML);
        }
        else if(stringXML.contains("<Messages", Qt::CaseInsensitive))
        {
            emit messagesReceived(&stringXML);
        }
    }
    else
    {
        // handle errors here
        emit commError(&QString("Network error: ").append(reply->errorString()));
    }

    reply->deleteLater();
}
