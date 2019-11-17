#include <speech/speech.h>
#include <messages.h>
#include <QTextEdit>

class server_comm : protected speech::tcp::tcp_receiver<new_user_joined , new_message , disconnected , chat_error>
{
public:

    server_comm( QTextEdit* messages_display , QTextEdit* users_display , std::shared_ptr<QTcpSocket> s );

protected:

    void on_receive( const new_user_joined& ) override;
    void on_receive( const new_message& ) override;
    void on_receive( const disconnected& ) override;
    void on_receive( const chat_error& ) override;

private:

    QTextEdit* m_messages_display {};
    QTextEdit* m_users_display {};

    const std::vector< QString > m_colors;

};
