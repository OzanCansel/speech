#include "uniqueness.h"
#include <QDebug>

bool UniquenessController::check(std::string typeName, size_t hashCode)
{
    using namespace std;

    static std::vector<std::pair<std::string , size_t>> mTypes;
    static std::mutex mMutex;

    mMutex.lock();

    qDebug() << QString::fromStdString(typeName) << " " << hashCode;

    auto existedEntry = std::find_if(mTypes.begin() , mTypes.end() , [&typeName](const auto& pair) {
        return pair.first == typeName;
    });

    //If type exists but doesn't match hashes
    if(existedEntry != mTypes.end() &&
            existedEntry->second != hashCode)
    {
        std::string errorMessage = "For the type of ";
        errorMessage += typeName;
        errorMessage += " hashes doesn't match. { ";
        errorMessage += to_string(hashCode);
        errorMessage += " != ";
        errorMessage += to_string(existedEntry->second);
//        throw logic_error(errorMessage);

        return false;
    }

    auto clashingEntry = find_if(mTypes.begin() , mTypes.end() , [&typeName , &hashCode](const auto& pair){
        return pair.first != typeName && pair.second == hashCode;
    });

    //IF type exists but doesn't match hashes
    if(clashingEntry != mTypes.end())
    {
        std::string errorMessage = "Hash codes of types ";
        errorMessage += typeName;
        errorMessage += " and ";
        errorMessage += clashingEntry->first;
        errorMessage += " are clashing!";
//        throw logic_error(errorMessage);

        return false;
    }

    mTypes.push_back(pair<string , size_t>(typeName , hashCode));

    mMutex.unlock();

    return true;
}
