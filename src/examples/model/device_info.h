#ifndef SPEECH_DEVICE_INFO_H
#define SPEECH_DEVICE_INFO_H

#include <QDataStream>
#include <QDebug>
#include <QString>

struct device_info {
     QString host_name;
     QString mac_addr;
     QString distro;
     QString abi;
     QString cpu_arch;
     QString kernel_type;
     QString kernel_version;
};

QDataStream& operator<< ( QDataStream& out, const device_info& device )
{
     return out << device.host_name << device.mac_addr << device.distro << device.abi
            << device.cpu_arch << device.kernel_type << device.kernel_version;
}

QDataStream& operator>> ( QDataStream& in, device_info& device )
{
     return in >> device.host_name >> device.mac_addr >> device.distro >> device.abi
            >> device.cpu_arch >> device.kernel_type >> device.kernel_version;
}

QDebug operator<< ( QDebug out, const device_info& device )
{
     return out << "{ host :" << device.host_name
            << " mac_addr :" << device.mac_addr
            << " distro :" << device.distro
            << " abi :" << device.abi
            << " arch :" << device.cpu_arch
            << " kernel :" << QString ( device.kernel_type + device.kernel_version )
            << "}";
}

#endif
