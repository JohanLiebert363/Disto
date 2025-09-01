#ifndef CLIENT_H
#define CLIENT_H

#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include <QTcpSocket>
#include <QString>
#include <QHostAddress>

#define PORT 8080

class ChatClient : public QWidget {
    Q_OBJECT

public:
    ChatClient(QWidget *parent = nullptr);

private slots:
    void connectToServer();
    void sendMessage();
    void readMessage();

private:
    QTcpSocket *socket;
    QLineEdit *serverAddressInput;
    QLineEdit *messageInput;
    QTextEdit *chatDisplay;
    QPushButton *connectButton;
    QPushButton *sendButton;
};

#endif // CLIENT_H

#include "client.h"
#include <QVBoxLayout>
#include <QHostAddress>

ChatClient::ChatClient(QWidget *parent) : QWidget(parent), socket(new QTcpSocket(this)) {
    QVBoxLayout *layout = new QVBoxLayout(this);

    // Server address input
    serverAddressInput = new QLineEdit(this);
    serverAddressInput->setPlaceholderText("Enter server IPv6 address (e.g., 2a06:63c4:e00:2d00:ea7e:abb3:71f2:b918)");
    layout->addWidget(serverAddressInput);

    // Connect button
    connectButton = new QPushButton("Connect to Server", this);
    layout->addWidget(connectButton);

    // Chat display
    chatDisplay = new QTextEdit(this);
    chatDisplay->setReadOnly(true);
    layout->addWidget(chatDisplay);

    // Message input
    messageInput = new QLineEdit(this);
    messageInput->setPlaceholderText("Enter your message...");
    layout->addWidget(messageInput);

    // Send button
    sendButton = new QPushButton("Send", this);
    sendButton->setEnabled(false); // Disabled until connected
    layout->addWidget(sendButton);

    // Connect signals and slots
    connect(connectButton, &QPushButton::clicked, this, &ChatClient::connectToServer);
    connect(sendButton, &QPushButton::clicked, this, &ChatClient::sendMessage);
    connect(socket, &QTcpSocket::readyRead, this, &ChatClient::readMessage);
}

void ChatClient::connectToServer() {
    QString serverAddress = serverAddressInput->text();
    if (serverAddress.isEmpty()) {
        chatDisplay->append("Please enter a server address.");
        return;
    }

    socket->connectToHost(QHostAddress(serverAddress), PORT);

    if (socket->waitForConnected(3000)) {
        chatDisplay->append("Connected to the server!");
        connectButton->setEnabled(false);
        sendButton->setEnabled(true);
    } else {
        chatDisplay->append("Failed to connect to the server.");
    }
}

void ChatClient::sendMessage() {
    QString message = messageInput->text();
    if (message.isEmpty()) {
        return;
    }

    socket->write(message.toUtf8());
    chatDisplay->append("You: " + message);
    messageInput->clear();
}

void ChatClient::readMessage() {
    QString message = QString::fromUtf8(socket->readAll());
    chatDisplay->append("Server: " + message);
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    ChatClient client;
    client.setWindowTitle("Chat Client");
    client.resize(400, 300);
    client.show();

    return app.exec();
}
