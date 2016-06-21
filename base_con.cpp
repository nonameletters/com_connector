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
        // cout << "Base connector distructor." << endl;
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
    QSqlDatabase CBaseCon::getDatabase()
    {
        return QSqlDatabase::database(getConnectionName());
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
        cout << "IN CONNECT: Connection name: " << getConnectionName().toStdString() << endl;

        QSqlDatabase m_db;

        if (!QSqlDatabase::contains(getConnectionName()))
        {
            m_db = QSqlDatabase::addDatabase(m_driverName, getConnectionName());
        }
//        else
//        {
//            cout << " Connection: " << getConnectionName().toStdString() << " already exist." << endl;
//        }
        m_db.setDatabaseName(m_dbName);
        m_db.setUserName(m_uName);
        m_db.setPassword(m_uPass);

        // cout << "Driver name: " << m_driverName.toStdString().c_str() << endl;
        // cout << "Database name: " << m_dbName.toStdString().c_str() << endl;

        bool res = false;

//        if (m_db.isValid() == false)
//        {
//            cout << "IS VALID FALSE" << endl;
//        }
//        else
//        {
//            cout << "IS VALID TRUE" << endl;
//        }

        if (m_db.isOpen() == false)
        {
            res = m_db.open();
//            cout << "DB is CLOSED" << endl;
        }
        else
        {
            res = true;
//            cout << "DB is OPENED" << endl;
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
            cout << "Driver name: " << m_driverName.toStdString() << endl;

            // resStr = m_db.lastError().text();
            resStr = m_db.lastError().text();
        }

        m_conResult = resStr;

//        cout << "From CONNECT: " << m_conResult.toStdString() << endl;

        return resStr;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    void CBaseCon::close()
    {
//        cout << "// ---------- ---------- ---------- ---------- ---------- ----------" << endl;
//        cout << "Close connection." << endl;
        //  m_db.close();
        // getDatabase().close();
        QStringList l_con = QSqlDatabase::connectionNames();
        auto it = begin(l_con);
        for (; it != end(l_con); it++)
        {
//            cout << "Con name: " << (*it).toStdString() << endl;
            {
                QSqlDatabase l_db = QSqlDatabase::database(*it, false);
                l_db.close();
            }
            QSqlDatabase::removeDatabase(*it);

        }

//        QStringList l_driver = QSqlDatabase::connectionNames();
//        it = begin(l_driver);
//        for (; it != end(l_driver); it++)
//        {
//            cout << "Driver name: " << (*it).toStdString() << endl;
//        }

//        cout << "GetConnectionName: " << getConnectionName().toStdString() << endl;
//        QSqlDatabase::removeDatabase(getConnectionName());

//        cout << "SECOND ITERATION" << endl;

//        l_con = QSqlDatabase::connectionNames();
//        it = begin(l_con);
//        for (; it != end(l_con); it++)
//        {
//            cout << "Con name2: " << (*it).toStdString() << endl;
//            // QSqlDatabase::removeDatabase(*it);
//        }

//        l_driver = QSqlDatabase::connectionNames();
//        it = begin(l_driver);
//        for (; it != end(l_driver); it++)
//        {
//            cout << "Driver name2: " << (*it).toStdString() << endl;
//        }

        // getDatabase().close();
//        cout << "END BASE CONSTRUCTOR ---------- ---------- ---------- ---------- ---------- ----------" << endl;
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
