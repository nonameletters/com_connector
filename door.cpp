#include "door.h"

namespace db_connector
{
    // ---------- ---------- ---------- ---------- ---------- ----------
    CDoor::CDoor():m_id(""),m_dUseForFact(""), m_defaultClose(""), m_doorName(""),
                   m_isDefault(""),m_enterMode(""),m_useTimeInterval(""), m_identType("")
    {
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    void CDoor::setId(const QString& value)
    {
        m_id = value;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    void CDoor::setUseForFact(const QString& value)
    {
        m_dUseForFact = value;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    void CDoor::setDefaultClose(const QString& value)
    {
        m_defaultClose = value;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    void CDoor::setDoorName(const QString& value)
    {
        m_doorName = value;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    void CDoor::setIsDefault(const QString& value)
    {
        m_isDefault = value;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    void CDoor::setEnterMode(const QString& value)
    {
        m_enterMode = value;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    void CDoor::setUseTimeInterval(const QString& value)
    {
        m_useTimeInterval = value;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    void CDoor::setIdentType(const QString& value)
    {
        m_identType = value;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    QString CDoor::getId() const
    {
        return m_id;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    QString CDoor::getUseForFact() const
    {
        return m_dUseForFact;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    QString CDoor::getDefaultClose() const
    {
        return m_defaultClose;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    QString CDoor::getDoorName() const
    {
        return m_doorName.trimmed();
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    QString CDoor::getIsDefault() const
    {
        return m_isDefault;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    QString CDoor::getEnterMode() const
    {
        return m_enterMode;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    QString CDoor::getUseTimeInterval() const
    {
        return m_useTimeInterval;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    QString CDoor::getIdentType() const
    {
        return m_identType;
    }
}
