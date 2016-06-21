#include "dolgnost.h"

namespace db_connector
{
    // ---------- ---------- ---------- ---------- ---------- ----------
    CDolgnost::CDolgnost(const QString& recId, const QString& id, const QString& name):m_recId(recId), m_id(id), m_name(name)
    {
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    void CDolgnost::setRecId(const QString& value)
    {
        m_recId = value.trimmed();
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    void CDolgnost::setId(const QString& value)
    {
        m_id = value.trimmed();
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    void CDolgnost::setName(const QString& value)
    {
        m_name = value.trimmed();
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    QString CDolgnost::getRecId() const
    {
        return m_recId.trimmed();
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    QString CDolgnost::getId()    const
    {
        return m_id.trimmed();
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    QString CDolgnost::getName()  const
    {
        return m_name.trimmed();
    }
}
