#ifndef DOLGNOST_H
#define DOLGNOST_H

#include <QString>


namespace db_connector
{
    class CDolgnost
    {
        public:
            CDolgnost(const QString& recId = "", const QString& id = "", const QString& name = "");

            void setRecId(const QString& value);
            void setId(const QString& value);
            void setName(const QString& value);

            QString getRecId() const;
            QString getId()    const;
            QString getName()  const;

        private:
            QString m_recId;
            QString m_id;
            QString m_name;
    };
}

#endif // DOLGNOST_H
