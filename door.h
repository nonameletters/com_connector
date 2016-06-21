#ifndef DOOR_H
#define DOOR_H

#include <QString>

namespace db_connector
{
    class CDoor
    {
        public:
            CDoor();

            void setId(const QString& value);
            void setUseForFact(const QString& value);
            void setDefaultClose(const QString& value);
            void setDoorName(const QString& value);
            void setIsDefault(const QString& value);
            void setEnterMode(const QString& value);
            void setUseTimeInterval(const QString& value);
            void setIdentType(const QString& value);

            QString getId() const;
            QString getUseForFact() const;
            QString getDefaultClose() const;
            QString getDoorName() const;
            QString getIsDefault() const;
            QString getEnterMode() const;
            QString getUseTimeInterval() const;
            QString getIdentType() const;

        private:
            QString m_id;
            QString m_dUseForFact;
            QString m_defaultClose;
            QString m_doorName;
            QString m_isDefault;
            QString m_enterMode;
            QString m_useTimeInterval;
            QString m_identType;
    };
}

#endif // DOOR_H
