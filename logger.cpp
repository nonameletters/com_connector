#include "logger.h"

namespace ns_logger
{
    // ---------- ---------- ---------- ---------- ---------- ----------
    CLogger* CLogger::m_instance = nullptr;

    // ---------- ---------- ---------- ---------- ---------- ----------
    CLogger* CLogger::getInstance()
    {
        if (m_instance == nullptr)
        {
            m_instance = new CLogger();
        }

        return m_instance;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    void CLogger::log(const char* mess)
    {
        m_log << mess << endl;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    void CLogger::log(const string& str)
    {
        m_log << str << endl;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    void CLogger::log(const QString& str)
    {
        m_log << str.toStdString() << endl;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    void CLogger::log(const QByteArray& str)
    {
        m_log << str.data() << endl;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    void CLogger::close()
    {
        m_log.close();
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    CLogger::~CLogger()
    {
        if (m_instance != nullptr)
        {
            delete m_instance;
        }
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    CLogger* CLogger::operator<<(const char* str)
    {
        m_log << str << endl;
        return getInstance();
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
//    CLogger* operator<<(CLogger* l, const char* str)
//    {
//        l->log(str);
//        return l;
//    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    CLogger* operator<<(CLogger* l, const string& str)
    {
        l->log(str);
        return l;
    }


    // ---------- ---------- ---------- ---------- ---------- ----------
    CLogger* operator<<(CLogger* l, const QString& str)
    {
        l << str.toStdString();
        return l;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    CLogger* operator<<(CLogger* l, const QByteArray& str)
    {
        l->log(str);
        return l;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
//    CLogger* operator << (CLogger* l, const CPerson& p)
//    {
//        l << p.getFirstName();
//        return l;
//    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    CLogger::CLogger()
    {
        m_log.open("anviz_sync.log", ios_base::out | ios_base::trunc);
    }
}
