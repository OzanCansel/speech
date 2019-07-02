#include <QObject>
#include <QByteArray>
#include <QDataStream>
#include <vector>
#include <memory>

namespace speech
{

template<typename Socket>
shared_socket<Socket>::shared_socket ( Socket &socket )
     : m_socket{new speech::handle::handle<Socket>{socket}}
{
     listen();
}

template<typename Socket>
shared_socket<Socket>::shared_socket ( std::shared_ptr<Socket> socket )
     : m_socket{new speech::handle::shared_ptr_handle<Socket>{socket}}
{
     listen();
}

template<typename Socket>
shared_socket<Socket>::shared_socket ( std::unique_ptr<Socket> socket )
     : m_socket{new speech::handle::unique_ptr_handle<Socket>{std::move ( socket ) }}
{
     listen();
}

template<typename Socket>
Socket& shared_socket<Socket>::socket()
{
     return m_socket->ref();
}

template<typename Socket>
void shared_socket<Socket>::attach ( std::function<int ( const QByteArray & ) > observer )
{
     m_listeners.push_back ( observer );
}

namespace impl
{
QByteArray read_all ( QTcpSocket& socket )
{
     return socket.readAll();
}

QByteArray read_all ( QUdpSocket& socket )
{
     QByteArray datagram;

     datagram.resize ( socket.pendingDatagramSize() );

     //Read datagram
     QHostAddress client_address;
     quint16 client_port;
     socket.readDatagram ( datagram.data(), datagram.size(), &client_address, &client_port );

     return datagram;
}
}

template<typename Socket>
void shared_socket<Socket>::listen()
{
     QObject::connect ( &m_socket->ref(), &Socket::readyRead, [this] {
          on_data_received();
     } );
}

template<typename Socket>
void shared_socket<Socket>::on_data_received()
{
     int parsed_data_length{};
     auto& sck = m_socket->ref();

     while ( ( !m_buffer.isEmpty() && parsed_data_length > 0 ) || sck.bytesAvailable() ) {

          m_buffer.append ( impl::read_all ( sck ) );
          bool invalid_msg{false};
          int could_not_be_parsed_count{};
          parsed_data_length = 0;

          //Call listeners
          for ( auto f : m_listeners ) {
               auto processed_bytes = f ( m_buffer );

               if ( processed_bytes == -1 ) {
                    could_not_be_parsed_count++;
               } else if ( processed_bytes == -2 ) {
                    invalid_msg = true;
                    break;
               } else if ( processed_bytes > parsed_data_length ) {
                    parsed_data_length = processed_bytes;
               }
          }

          auto could_not_be_parsed = could_not_be_parsed_count == m_listeners.size();

          const static QByteArray start_token = []() {
               QByteArray arr;
               QDataStream ss ( &arr, QIODevice::WriteOnly );
               ss.setVersion ( QDataStream::Qt_5_0 );
               ss << 241994 << 1511999 << 991973;
               return arr;
          }
          ();

          if ( invalid_msg || could_not_be_parsed ) {

               if ( could_not_be_parsed ) {
                    m_buffer.remove ( 0, start_token.size() );
               }

               //discard current message and jump to next message
               auto start_of_msg_idx = m_buffer.indexOf ( start_token );
               auto invalid_data_len = start_of_msg_idx == -1 ? m_buffer.size() : start_of_msg_idx;
               m_buffer.remove ( 0, invalid_data_len );
          } else {
               m_buffer.remove ( 0, parsed_data_length );
          }
     }
}

} // namespace speech
