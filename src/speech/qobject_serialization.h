#ifndef SPEECH_QOBJECT_SERIALIZATION_H
#define SPEECH_QOBJECT_SERIALIZATION_H

#include <QObject>
#include <QDataStream>
#include <QDebug>
#include <QMetaObject>
#include <QMetaProperty>
#include <type_traits>

namespace speech
{
    template<class T>
    typename std::enable_if<std::is_base_of<QObject , T>::value , QDataStream&>::type operator<<(QDataStream& out , const T& obj)
    {
        qDebug() << "auto_serialize_out";
        auto meta_info = obj.metaObject();

        for(auto i = 0; i < meta_info->propertyCount(); ++i)
        {
            auto prop = meta_info->property(i);

            if(prop.isConstant() || !prop.isWritable() || !prop.isReadable())
                continue;

            qDebug() << prop.name() << " <= " << prop.read(&obj);

            out << prop.read(&obj);
        }

        return out;
    }

    template<class T>
    typename std::enable_if<std::is_base_of<QObject , T>::value , QDataStream&>::type operator>>(QDataStream& in , T& obj)
    {
        qDebug() << "auto_serialize_in";
                auto meta_info = obj.metaObject();

        for(auto i = 0; i < meta_info->propertyCount(); ++i)
        {
            auto prop = meta_info->property(i);

            if(prop.isConstant() || !prop.isWritable() || !prop.isReadable())
                continue;

            QVariant variant;
            in >> variant;

            qDebug() << prop.name() << " => " << variant;

            prop.write(&obj , variant);
        }

        return in;
    }

    template<class T>
    typename std::enable_if<std::is_base_of<QObject , T>::value , QDebug>::type operator<<(QDebug out , const T& obj)
    {
        return out;
    }
}

#endif