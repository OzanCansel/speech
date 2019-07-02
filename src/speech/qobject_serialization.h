#ifndef SPEECH_QOBJECT_SERIALIZATION_H
#define SPEECH_QOBJECT_SERIALIZATION_H

#include <QObject>
#include <QDataStream>
#include <QDebug>
#include <QMetaObject>
#include <QMetaProperty>
#include <vector>
#include <type_traits>

namespace speech
{
template<class T>
typename std::enable_if<std::is_base_of<QObject, T>::value, QDataStream&>::type operator<< ( QDataStream& out, const T& obj )
{
     auto meta_info = obj.metaObject();

     std::vector<int> writable_property_indexes;

     for ( auto i = 0; i < meta_info->propertyCount(); ++i ) {
          auto prop = meta_info->property ( i );

          if ( prop.isConstant() || !prop.isWritable() || !prop.isReadable() ) {
               continue;
          }

          writable_property_indexes.push_back ( i );
     }

     out << static_cast<int> ( writable_property_indexes.size() );

     for ( auto index : writable_property_indexes ) {
          auto prop = meta_info->property ( index );

          out << QString::fromLatin1 ( prop.name() );
          out << prop.read ( &obj );
     }

     return out;
}

template<class T>
typename std::enable_if<std::is_base_of<QObject, T>::value, QDataStream&>::type operator>> ( QDataStream& in, T& obj )
{
     auto meta_info = obj.metaObject();

     int count_of_properties{ };
     in >> count_of_properties;

     for ( auto i = 0; i < count_of_properties; ++i ) {

          //Read property name and value
          QString prop_name;
          QVariant prop_val;

          in >> prop_name;
          in >> prop_val;

          auto prop_idx = meta_info->indexOfProperty ( prop_name.toStdString().c_str() );

          if ( prop_idx < 0 ) {
               continue;
          }

          auto prop = meta_info->property ( prop_idx );

          if ( prop.isConstant() || !prop.isWritable() || !prop.isReadable() ) {
               continue;
          }

          prop.write ( &obj, prop_val );
     }

     return in;
}

template<class T>
typename std::enable_if<std::is_base_of<QObject, T>::value, QDebug>::type operator<< ( QDebug out, const T& obj )
{

     auto meta_info = obj.metaObject();

     for ( auto i = 0; i < meta_info->propertyCount(); ++i ) {
          auto prop = meta_info->property ( i );

          if ( !prop.isReadable() ) {
               continue;
          }

          auto prop_name = prop.name();
          auto prop_value = prop.read ( &obj );

          out << prop_name << " : " << prop_value << " , ";
     }

     return out;
}
}

#endif
