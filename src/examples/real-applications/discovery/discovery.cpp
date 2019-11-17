#include <QDataStream>
#include <QCoreApplication>
#include <QHostAddress>
#include <QCommandLineParser>
#include <QSysInfo>
#include <QHostInfo>
#include <QTimer>
#include <QDebug>
#include <QNetworkInterface>
#include <speech/speech.h>
#include <device_info.h>
#include <vector>
#include <algorithm>

QString retrieve_mac_addr()
{
     for ( const QNetworkInterface& netInterface : QNetworkInterface::allInterfaces() ) {
          // Return only the first non-loopback MAC Address
          if ( ! ( netInterface.flags() & QNetworkInterface::IsLoopBack ) ) {
               return netInterface.hardwareAddress();
          }
     }

     return QString();
}

class discovery : speech::udp::udp_receiver<device_info>
{
public:

     explicit discovery ( speech::port p )
        : speech::udp::udp_receiver<device_info> { p }
     {
          m_broadcast_tick.setInterval ( 1000 );

          QObject::connect ( &m_broadcast_tick, &QTimer::timeout, [&]() {
               using namespace speech;
               using namespace speech::udp;

               transmit
               (
               device_info {
                    QHostInfo::localHostName(),
                    retrieve_mac_addr(),
                    QSysInfo::prettyProductName(),
                    QSysInfo::buildAbi(),
                    QSysInfo::currentCpuArchitecture(),
                    QSysInfo::kernelType(),
                    QSysInfo::kernelVersion()
               },
               QHostAddress ( QHostAddress::Broadcast ), p
               );
          } );

          m_broadcast_tick.start();
     }

     void on_receive ( const device_info& device ) override
     {
          bool exist = std::any_of ( m_devices.begin(), m_devices.end(), [&device] ( const auto& d ) {
               return d.mac_addr == device.mac_addr;
          } );
          if ( !exist ) {
               qDebug() << "++ new device" << device;
               m_devices.push_back ( device );
          }
     }

private:

     QTimer m_broadcast_tick;
     std::vector<device_info> m_devices;

};

int main ( int argc, char **argv )
{
     using namespace speech::udp;
     using namespace std;

     QCoreApplication app ( argc, argv );

     QCommandLineParser parser;
     parser.addHelpOption();

     parser.addOptions ( {
          { {"p", "port"}, "Specify port number", "port number" }
     }
                       );

     parser.process ( app );

     //Defaults
     auto port = 24942;

     if ( parser.isSet ( "p" ) ) {
          port = parser.value ( "p" ).toInt();
     }

     discovery d { speech::port ( port ) };

     return QCoreApplication::exec();
}
