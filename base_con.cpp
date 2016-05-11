#include "base_con.h"

namespace db_connector
{
    // ---------- ---------- ---------- ---------- ---------- ----------
    CBaseCon::CBaseCon()
    {
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    CBaseCon::~CBaseCon()
    {
        close();
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    QString CBaseCon::getConnectionName()
    {
        return QString("Base connection");
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    QStringList CBaseCon::getDrivers()
    {
        return QSqlDatabase::drivers();
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    QString CBaseCon::getConResult() const
    {
        return m_conResult;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    QSqlDatabase& CBaseCon::getDatabase()
    {
        return m_db;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    void CBaseCon::setHostName(QString host)
    {
        cout << "CDbConnector setHostName" << endl;
        m_host = host;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    void CBaseCon::setDatabaseName(QString dbName)
    {
        cout << "CDbConnector setDatabaseName" << endl;
        m_dbName = dbName;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    void CBaseCon::setDriverName(QString driver)
    {
        m_driverName = driver;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    void CBaseCon::setUsername(QString user)
    {
        m_uName = user;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    void CBaseCon::setPassword(QString pass)
    {
        m_uPass = pass;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
//    void CBaseCon::setStartDateTime(const QDateTime& dt)
//    {
//        m_startDT = dt;
//    }

    // ---------- ---------- ---------- ---------- ---------- ----------
//    void CBaseCon::setEndDateTime(const QDateTime& dt)
//    {
//        m_endDT = dt;
//    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    QString CBaseCon::connect()
    {
        cout << "Connection name: " << getConnectionName().toStdString() << endl;

        if (!QSqlDatabase::contains(getConnectionName()))
        {
            m_db = QSqlDatabase::addDatabase(m_driverName, getConnectionName());
        }
        m_db.setDatabaseName(m_dbName);
        m_db.setUserName(m_uName);
        m_db.setPassword(m_uPass);

        // cout << "Driver name: " << m_driverName.toStdString().c_str() << endl;
        // cout << "Database name: " << m_dbName.toStdString().c_str() << endl;

        bool res = false;

        if (m_db.isOpen() == false)
        {
            res = m_db.open();
        }
        else
        {
            res = true;
        }

        QString resStr;

        if (res == true)
        {
            resStr = "Connection success";
        }
        else
        {
            cout << "Database name: " << m_dbName.toStdString().c_str() << endl;
            cout << "User name: " << m_uName.toStdString().c_str() << endl;
            cout << "User password: " << m_uPass.toStdString().c_str() << endl;

            // resStr = m_db.lastError().text();
            resStr = m_db.lastError().text();
        }

        m_conResult = resStr;

        // cout << "From CONNECT: " << m_conResult.toStdString() << endl;

        return resStr;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    void CBaseCon::close()
    {
        getDatabase().close();
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    bool CBaseCon::execQuery()
    {
        return false;// execQueryEmployee("Кобрусев");
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    bool CBaseCon::execUpdateEvents(QString query)
    {
        QSqlQuery l_query(getDatabase());


        QString l_curQuery = query;
        if (l_query.exec(l_curQuery) == false)
        {
            cout << "Query FAULT: " << l_curQuery.toStdString().c_str() << endl;
            return false;
        }

        return true;
    }
}
