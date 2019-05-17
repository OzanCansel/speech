#include <QCoreApplication>
#include <QUuid>
#include <QCommandLineParser>
#include <speech/tcp/tcp_server.h>
#include <speech/tcp/file_transmitter.h>
#include <speech/tcp/tcp_transmitter.h>
#include <QDateTime>
#include <QTime>

class handler_1 : public speech::tcp::tcp_receiver<speech::impl::f_start , speech::impl::f_data , speech::impl::f_finish>
{

    public:

        handler_1(speech::shared_socket<QTcpSocket>& sck) : 
            speech::tcp::tcp_receiver<speech::impl::f_start , speech::impl::f_data , speech::impl::f_finish>{ sck } ,
            m_tcp{ sck.socket() }
        {   
            qDebug() << "Handler_1 constructed. " << id.toString();
        }

        ~handler_1() override { 
            qDebug() << "Handler_1 destroyed. " << id.toString();
        }

    protected:

        void on_receive(const speech::impl::f_start& s) override
        {
            
            qDebug() << "File start " << s.id << " " << s.f_name;
            m_started = QDateTime::currentDateTime();
            qDebug() << m_started.toString(Qt::ISODate);

            m_file.setFileName(s.f_name);
            if(!m_file.open(QIODevice::WriteOnly))
            {
                qDebug() << "Error could not write to the file";
            }

            m_file_size = s.size;

            m_chronometer.start();

        }

        void on_receive(const speech::impl::f_data& d) override
        {
            m_file.write(d.data);
            m_tcp.transmit({ true });
            m_received += d.data.size();

            qDebug() << "Received " << m_received << "/" << m_file_size;
        }

        void on_receive(const speech::impl::f_finish& f) override
        {
            qDebug() << "File finish " << f.id;
            qDebug() << QDateTime::currentDateTime().toString(Qt::ISODate);
            qDebug() << m_chronometer.elapsed() << "ms";
            m_file.waitForBytesWritten(-1);
            m_file.close();
        }

    private:

        QDateTime m_started;
        QUuid id { QUuid::createUuid() };
        QFile m_file;
        QTime m_chronometer;
        int m_file_size{};
        int m_received{};
        speech::tcp::tcp_transmitter<speech::impl::f_ok> m_tcp;

};

int main(int argc, char** argv)
{
    QCoreApplication app(argc , argv);

    using namespace speech;
    using namespace speech::tcp;

    QCommandLineParser parser;
    parser.addHelpOption();

    parser.addOptions(
        {{ {"p", "port"} , "Specify listening port number" , "port number" }});

    parser.process(app);

    //Defaults
    auto port = 24942;

    if (parser.isSet("p"))
        port = parser.value("p").toInt();

    auto sw = make_server(QHostAddress::Any , speech::port{ port } , 
        make_handler<handler_1>());

    return app.exec();
}