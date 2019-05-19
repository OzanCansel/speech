#ifndef SPEECH_FILE_TRANSMITTER_H
#define SPEECH_FILE_TRANSMITTER_H

#include <QUuid>
#include <QDataStream>
#include <QFile>
#include <QFileInfo>
#include <QThread>
#include "speech/tcp/tcp_transmitter.h"
#include "speech/tcp/tcp_receiver.h"

namespace speech
{
    namespace impl
    {
        struct f_start
        {
            QUuid id { QUuid::createUuid() };
            QString f_name;
            int size{};
        };

        struct f_data
        {
            QUuid id;
            QByteArray data;
        };

        struct f_finish
        {
            QUuid id;
        };

        struct f_ok
        {   
            bool received{ false };
        };

        QDataStream& operator<<(QDataStream& out, const f_start& e)
        {
            return out << e.id << e.f_name << e.size; 
        }
        
        QDataStream& operator<<(QDataStream& out, const f_data& e)
        {
            return out << e.id << e.data;
        }

        QDataStream& operator<<(QDataStream& out, const f_finish& e)
        {
            return out << e.id;
        }

        QDataStream& operator<<(QDataStream& out, const f_ok& e)
        {
            return out << e.received;
        }

        QDataStream& operator>>(QDataStream& in, f_start& e)
        {
            return in >> e.id >> e.f_name >> e.size;
        }

        QDataStream& operator>>(QDataStream& in, f_data& e)
        {
            return in >> e.id >> e.data;
        }

        QDataStream& operator>>(QDataStream& in, f_finish& e)
        {
            return in >> e.id;
        }


        QDataStream& operator>>(QDataStream& in, f_ok& e)
        {
            return in >> e.received;
        }

    }

    class file_transmitter 
        : 
        speech::tcp::tcp_transmitter<impl::f_start , impl::f_data , impl::f_finish> , speech::tcp::tcp_receiver<impl::f_ok>
    {
        public:

            file_transmitter(shared_socket<QTcpSocket>& socket , QHostAddress host , speech::port p)
                :
                speech::tcp::tcp_transmitter<impl::f_start , impl::f_data , impl::f_finish>{ socket.socket(), host , p } ,
                speech::tcp::tcp_receiver<impl::f_ok> { socket }
            {   }

            bool send(QString file)
            {

                if(m_file.isOpen())
                    return false;

                m_file.setFileName(file);

                if(!m_file.open(QIODevice::ReadOnly))
                    return false;

                impl::f_start s;
                s.f_name = QFileInfo(file).fileName();
                s.size = m_file.bytesAvailable();
                transmit(s);
                m_start = s;

                m_total_size = m_file.bytesAvailable();
                if(m_file.isReadable() && m_file.bytesAvailable())
                {
                    impl::f_data d;
                    d.id = s.id;
                    d.data = m_file.read(m_packet_size);
                    m_sent += d.data.size();
                    transmit(d);

                    qDebug() << "Sent " << m_sent << "/" << m_total_size;
                }

                return true;
            }

            void on_receive(const impl::f_ok& ok)
            {
                if(m_file.isReadable() && m_file.bytesAvailable())
                {
                    impl::f_data d;
                    d.id = m_start.id;
                    d.data = m_file.read(m_packet_size);
                    m_sent += d.data.size();
                    transmit(d);

                    qDebug() << "Sent " << m_sent << "/" << m_total_size;
                }
                else
                {
                    impl::f_finish finish;
                    finish.id = m_start.id;
                    transmit(finish);
                }

                // QThread::msleep(5);
            }

            int m_packet_size { 55000 };
            int m_sent{};
            int m_total_size{};
            QFile m_file;
            impl::f_start m_start;
    };
}

#endif