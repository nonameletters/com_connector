#ifndef BASE_CON_H
#define BASE_CON_H

#include <QString>
#include <QStringList>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <iostream>

using namespace std;

namespace db_connector
{
    class CBaseCon
    {
        public:
            CBaseCon();
            virtual ~CBaseCon();

            virtual QString getConnectionName();

            static QStringList getDrivers();
            void setDatabaseName(QString dbName);
            QString getConResult() const;
            QSqlDatabase& getDatabase();
            void setHostName(QString host);
            void setDriverName(QString driver);
            void setUsername(QString user);
            void setPassword(QString pass);

            // void setStartDateTime(const QDateTime& dt);
            // void setEndDateTime(const QDateTime& dt);

            QString connect();
            void    close();
            bool    execQuery();
            bool    execUpdateEvents(QString query);

        private:
            QString m_uName;
            QString m_uPass;
            QString m_driverName;
            QString m_dbName;
            QString m_connection;
            QString m_host;

            QString       m_conResult = "Connection was not set.";
            QSqlDatabase  m_db;
    };
}
#endif // BASE_CON_H
