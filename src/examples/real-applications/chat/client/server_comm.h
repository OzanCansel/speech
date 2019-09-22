#include <speech/speech.h>
#include <messages.h>
#include <QTextEdit>

class server_comm : protected speech::tcp::tcp_receiver<new_user_joined , new_message>
{
public:

    server_comm( QTextEdit* messages_display , std::shared_ptr<QTcpSocket> s );

protected:

    void on_receive( const new_user_joined& my_id ) override;
    void on_receive( const new_message& my_pos ) override;

private:

    QTextEdit* m_messages_display;
    const std::vector< QString > m_colors;

};
