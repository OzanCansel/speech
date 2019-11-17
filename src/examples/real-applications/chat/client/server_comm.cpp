#include "server_comm.h"
#include <QTimer>
#include <QCollator>
#include <QApplication>

server_comm::server_comm( QTextEdit* messages_display , QTextEdit* users_display , std::shared_ptr<QTcpSocket> s )
    :   speech::tcp::tcp_receiver<new_user_joined , new_message , disconnected, chat_error> { std::move( s ) }
    ,   m_messages_display { messages_display }
    ,   m_users_display { users_display }
    ,   m_colors { "#55286f" , "#06d6a0" , "#1b9aaa" , "#ef476f" , "#ffc43d" , "#2d3142" , "#941b0c" }
{}

void server_comm::on_receive( const new_user_joined& user )
{
    qDebug() <<  user;
    m_messages_display->append( QString { "<b style=\"color: %0\">%1</b> has joined." }
                                        .arg( m_colors.at( qrand() % m_colors.size() ) ) // Pick random color
                                        .arg( user.user_name ));

    auto users = m_users_display->toPlainText().split("\r" , QString::SkipEmptyParts );

    users.append( user.user_name );
    users.sort( Qt::CaseInsensitive );

    QCollator collator;
    collator.setNumericMode(true);

    std::sort( users.begin() , users.end() ,  [&collator](const QString &file1, const QString &file2)
    {
        return collator.compare(file1, file2) < 0;
    });

    m_users_display->setPlainText( users.join("\n") );
}

void server_comm::on_receive( const new_message& msg )
{
    qDebug() << msg;
    m_messages_display->append( QString { "<b style=\"color: %0\">%1 :</b> %2" }
                                .arg( m_colors.at( qrand() % m_colors.size() ) ) // Pick random color
                                .arg( msg.user )
                                .arg( msg.message ));
}

void server_comm::on_receive( const disconnected& d )
{
    qDebug() << d;
    m_messages_display->append( QString { "<b style=\"color: #941b0c\">%0</b> has been disconnected." }
                                        .arg( d.user_name ));

    auto users = m_users_display->toPlainText().split( "\n" , QString::SkipEmptyParts );

    auto idx = users.indexOf( d.user_name );

    if ( idx >= 0 )
        users.removeAt( idx );

    m_users_display->setPlainText( users.join("\n") );
}

void server_comm::on_receive( const chat_error& e )
{
    m_messages_display->append( QString { "<b style=\"color: #941b0c\">Error occurred : %0</b>" }
                                        .arg( e.message ));
    m_messages_display->append( QString { "<b style=\"color: #941b0c\">Exiting in 5 seconds</b>" } );

    QTimer::singleShot( 5000 , []() {
        QApplication::exit( 1 );
    });
}
