#include "car_info.h"

QString car_info::name()
{
     return m_name;
}

void car_info::set_name ( const QString& name )
{
     m_name = name;
}
