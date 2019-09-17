#ifndef SPEECH_DEVICE_INFO_H
#define SPEECH_DEVICE_INFO_H

#include <QDataStream>
#include <QDebug>
#include <QString>
#include <speech/speech.h>

struct device_info {
    QString host_name;
    QString mac_addr;
    QString distro;
    QString abi;
    QString cpu_arch;
    QString kernel_type;
    QString kernel_version;

    SPEECH_SERIALIZE( host_name , mac_addr , distro , abi
                      , cpu_arch , kernel_type , kernel_version )

};

#endif
