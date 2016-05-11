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
    CLogger::CLogger()
    {
        m_log.open("anviz_sync.log", ios_base::out | ios_base::trunc);
    }
}
