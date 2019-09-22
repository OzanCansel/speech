#include "server_comm.h"

server_comm::server_comm( QTextEdit* messages_display , std::shared_ptr<QTcpSocket> s )
    :   speech::tcp::tcp_receiver<new_user_joined , new_message> { std::move( s ) }
    ,   m_messages_display { messages_display }
    ,   m_colors { "#55286f" , "#06d6a0" , "#1b9aaa" , "#ef476f" , "#ffc43d" , "#2d3142" , "#941b0c" }
{}

void server_comm::on_receive( const new_user_joined& my_id )
{
    qDebug() <<  my_id;
    m_messages_display->append( QString { "<b style=\"color: %0\">%1</b> joined." }
                                        .arg( m_colors.at( qrand() % m_colors.size() ) ) // Pick random color
                                        .arg( my_id.user_name ));
}

void server_comm::on_receive( const new_message& msg )
{
    qDebug() << msg;
    m_messages_display->append( QString { "<b style=\"color: %0\">%1 :</b> %2" }
                                .arg( m_colors.at( qrand() % m_colors.size() ) ) // Pick random color
                                .arg( msg.user )
                                .arg( msg.message ));
}
