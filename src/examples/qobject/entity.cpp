#include "entity.h"

QString entity::name()
{
    return m_name;
}

void entity::set_name(QString name)
{
    m_name = name;
}

int entity::identity()
{
    return m_identity;
}

void entity::set_identity(int identity)
{
    m_identity = identity;
}