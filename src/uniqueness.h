#ifndef UNIQUENESS_H
#define UNIQUENESS_H

#include <algorithm>
#include <string>
#include <vector>
#include <mutex>

class UniquenessController
{

public:

    static bool check(std::string typeName , size_t hashCode);

};

#endif // UNIQUENESS_H
