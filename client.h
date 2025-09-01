#ifndef CLIENT_H
#define CLIENT_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include <QTcpSocket>

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