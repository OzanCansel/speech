#include <QApplication>
#include <QMainWindow>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QPushButton>
#include <speech/speech.h>
#include <messages.h>
#include "server_comm.h"
#include "send_event_text_edit.h"

int main( int argc , char** argv )
{

    using namespace speech;
    using namespace speech::tcp;

    QApplication app( argc , argv );

    QCommandLineParser parser;
    QCommandLineOption user_name_opt { { "u" , "user" } , "user_name in chat-client" , "user_name" , "Unknown"} ,
                        host_opt { { "i" , "ip" } , "ip address of host" , "host" , "127.0.0.1" } ,
                        port_opt { { "p" , "port" } , "port of host" , "port" , "24942" };
    parser.addHelpOption();

    parser.addOptions ( { user_name_opt , host_opt , port_opt } );
    parser.process ( app );

    auto nick_name = parser.value( user_name_opt );
    QHostAddress host_ip = QHostAddress { parser.value( host_opt ) };
    auto port = parser.value( port_opt ).toInt();

    if ( host_ip.isNull() )
    {
        qDebug() << "Ip is not suitable.";
        return 1;
    }

    if ( port <= 0 )
    {
        qDebug() << "Port cannot be zero or negative.";
        return 1;
    }

    QMainWindow client_view;

    QVBoxLayout* v_bottom_layout = new QVBoxLayout();
    QHBoxLayout* h_bottom_layout = new QHBoxLayout();
    QHBoxLayout* h_top_layout = new QHBoxLayout();
    send_event_text_edit *my_message = new send_event_text_edit();
    QTextEdit *incoming_messages = new QTextEdit();
    QTextEdit *connected_users = new QTextEdit();
    QPushButton *send_button = new QPushButton();

    connected_users->setReadOnly(true);
    incoming_messages->setReadOnly( true );
    h_top_layout->addWidget( incoming_messages , 9 );
    h_top_layout->addWidget( connected_users , 1 );

    send_button->setSizePolicy( QSizePolicy::Expanding , QSizePolicy::Expanding );
    send_button->setText( "Send" );

    h_bottom_layout->addWidget( my_message , 10 );
    h_bottom_layout->addWidget( send_button , 1 );

    v_bottom_layout->addLayout( h_top_layout , 10 );
    v_bottom_layout->addLayout( h_bottom_layout , 1 );

    client_view.setFixedWidth( 800 );
    client_view.setFixedHeight( 600 );

    QWidget* main_widget = new QWidget();
    main_widget->setLayout( v_bottom_layout );
    client_view.setCentralWidget( main_widget );
    client_view.setWindowTitle( QString { "chat-client [ %0 ]" }.arg( nick_name ) );
    client_view.show();

    std::shared_ptr<QTcpSocket> sck = std::make_shared<QTcpSocket>();
    server_comm comm { incoming_messages , connected_users , sck };

    // Initial connection
    transmit( let_me_join { nick_name } , host_ip , speech::port { port } , sck );

    QObject::connect( my_message , &send_event_text_edit::return_pressed , [ my_message , sck ]() {
        auto message = my_message->toPlainText();

        if ( message.isEmpty() )
            return;

        my_message->clear();

        speech::tcp::transmit( send_message { message } , sck );
    });

    // Send message handler
    QObject::connect( send_button , &QPushButton::clicked , [ my_message , sck ] ( bool ) {
        auto message = my_message->toPlainText();

        if ( message.isEmpty() )
            return;

        my_message->clear();

        speech::tcp::transmit( send_message { message } , sck );
    });

    return QApplication::exec();
}
