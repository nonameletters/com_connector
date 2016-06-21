#ifndef PROFILE_H
#define PROFILE_H

#include <QString>
#include <vector>

#include "door.h"

using namespace std;

namespace db_connector
{
    class CProfile
    {
        public:
            CProfile();

            bool isEmpty() const;
            void setId(const QString& value);
            void setName(const QString& value);
            void setEnterClose(const QString& value);
            void setIdentType(const QString& value);
            void setCode(const QString& value);
            void setDoors(const vector<CDoor> value);

            QString getId() const;
            QString getName() const;
            QString getEnterClose() const;
            QString getIdentType() const;
            QString getCode() const;
            vector<CDoor> getDoors() const;


        private:
            QString m_id;
            QString m_name;
            QString m_enterClose;
            QString m_identType;
            QString m_code;

            vector<CDoor> m_doors;
    };
}

#endif // PROFILE_H
