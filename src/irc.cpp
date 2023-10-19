#include "irc.h"

#include <iostream>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <cstring>
#include <netdb.h>

#include <QDebug>
#include <QRandomGenerator>

#include "secret.h" // provides OAUTH

using namespace std;

void test_irc()
{
        qDebug()<< "Holi from IRC";
        string server = "irc.chat.twitch.tv"; // network address
        int port = 6667; // server port
        string nick = "NICK Wiringpi"; // NICK raw
        string user = "USER Wirinpi randomtext israndom :My first c++ bot"; // USER raw

        struct sockaddr_in addr;
        struct hostent *host; 

        /** Get an ip address from the network to connect to **/
        host = gethostbyname(server.c_str());

        /** Fill the members of the socket structs required to connect **/
 
        addr.sin_addr.s_addr = *(unsigned long*)host->h_addr;
        addr.sin_family = AF_INET;
        addr.sin_port = htons((unsigned short)port);
        int sockd = socket(AF_INET, SOCK_STREAM, 0);

        /** Connect to address **/
        connect(sockd, (struct sockaddr *)&addr, sizeof(addr));
 
        string channel = "#labgluon";
        string message = "PASS oauth:" OAUTH "\r\nNICK testBot\r\nJOIN " + channel + "\r\n";
        send(sockd, message.c_str(), message.length(), 0);
       
        char sockbuff[4096]; // array to hold the incoming socket data
        while (1) { 
                memset(&sockbuff, '\0', sizeof(sockbuff)); // make sure sockbuff[] is empty
                recv(sockd, sockbuff, 4096, 0); // Recieve all the data from server to sockbuff[]
                qDebug() << sockbuff;
        }
}

void ext_connect(int s, sockaddr * addr, unsigned long len )
{
    connect(s, addr, len);
}

IRC::IRC(){
    socket = new QTcpSocket(this);
    receivedMessages = 0;
    connect(socket, SIGNAL(connected()), this, SLOT(connected()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(parseNewData()));
}


void IRC::Init()
{
    socket->connectToHost("irc.chat.twitch.tv", 6667);

    // we need to wait...
    if(!socket->waitForConnected(2000))
    {
        qDebug() << "Error: " << socket->errorString();
    }
}

void IRC::connected()
{
    QString channel = "#labgluon";
    QString message = "PASS oauth:" OAUTH "\r\nNICK testBot\r\nJOIN " + channel + "\r\n";
    //send(sockd, message.c_str(), message.length(), 0);
    socket->write(message.toUtf8());
}

void IRC::sendText(QString message)
{
    QString privMsg = "PRIVMSG #labgluon :" + message + "\r\n";
    writters.append("<font color=\"" + getColorForNickname("labgluon").name() +"\">labgluon</font>:" + message);

    socket->write(privMsg.toUtf8());
    emit newWritter();
}

void IRC::parseNewData()
{
    QString data = socket->readAll();

    // The first two messages are due to successful login
    if (receivedMessages < 2)
    {
        receivedMessages++;
        return;
    }

    int nickEnd = data.indexOf("!");
    int msgStart = data.indexOf("PRIVMSG #labgluon :");
    qDebug() << "Datos RAW: "<<  data;
    if( nickEnd != -1 || msgStart != -1)
    {
        // Offset from "PRIVMSG #labgluon :" start.
        msgStart += 19;

        int msgEnd = data.indexOf("\r\n", msgStart);
        if(msgEnd == -1)
        {
            msgEnd = data.length()-1;
        }

        // nickEnd points to the character "!"
        nickEnd = nickEnd - 1;
        /*QStringRef nickname(&data, 1, nickEnd);
        QStringRef message(&data, msgStart, msgEnd-msgStart);*/

        QString chatMessage = "<font color=\"" + getColorForNickname(data.mid(1,nickEnd)).name() +"\">" + data.mid(1,nickEnd) + "</font>:" + data.mid(msgStart, msgEnd-msgStart);
        //writters.append(data.mid(1, nickEnd));
        writters.append(chatMessage);
        emit newWritter();
    }

}


QColor IRC::getColorForNickname(QString nickname)
{
    if(nicknameColors.contains(nickname))
    {
        return nicknameColors[nickname];
    }



    QColor colorRandom;
    colorRandom.setHsv(
        (10*nicknameColors.count())%360 + generator.bounded(0,10),
        200,
        255
        );

    qDebug() << "Nuevo color generado: " << colorRandom.name();
    nicknameColors.insert(nickname, colorRandom);
    return colorRandom;
}