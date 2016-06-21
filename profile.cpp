#include "profile.h"

namespace db_connector
{
    // ---------- ---------- ---------- ---------- ---------- ----------
    CProfile::CProfile() : m_id(""), m_name(""), m_enterClose(""), m_identType(""), m_code("")
    {
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    bool CProfile::isEmpty() const
    {
        if ((m_id.trimmed().isEmpty()) || m_code.trimmed().isEmpty())
        {
            return true;
        }

        return false;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    void CProfile::setId(const QString& value)
    {
        m_id = value;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    void CProfile::setName(const QString& value)
    {
        m_name = value;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    void CProfile::setEnterClose(const QString& value)
    {
        m_enterClose = value;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    void CProfile::setIdentType(const QString& value)
    {
        m_identType = value;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    void CProfile::setCode(const QString& value)
    {
        m_code = value;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    void CProfile::setDoors(const vector<CDoor> value)
    {
        m_doors.assign(value.begin(), value.end());
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    QString CProfile::getId() const
    {
        return m_id;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    QString CProfile::getName() const
    {
        return m_name;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    QString CProfile::getEnterClose() const
    {
        return m_enterClose;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    QString CProfile::getIdentType() const
    {
        return m_identType;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    QString CProfile::getCode() const
    {
        return m_code;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    vector<CDoor> CProfile::getDoors() const
    {
        return m_doors;
    }
}
