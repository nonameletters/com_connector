#include "employee.h"

namespace nsComConnector
{
    // ---------- ---------- ---------- ---------- ---------- ----------
    CEmployee::CEmployee()
    {

    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    void CEmployee::setName(string name)
    {
        m_name = name;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    void CEmployee::setNumber(string number)
    {
        m_number = number;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    void CEmployee::setPosition(string position)
    {
        m_position = position;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    void CEmployee::setInn(string inn)
    {
        m_inn = inn;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    void CEmployee::setFailed(bool failed)
    {
        m_failed = failed;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    void CEmployee::setOrganisation(string org)
    {
        m_organisation = org;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    void CEmployee::setHireDate(QDate date)
    {
        m_hireDate = date;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    void CEmployee::setDismissionDate(QDate date)
    {
        m_dismissionDate = date;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    string CEmployee::getName() const
    {
        return m_name;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    string CEmployee::getNumber() const
    {
        return m_number;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    string CEmployee::getPosition() const
    {
        return m_position;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    string CEmployee::getInn() const
    {
        return m_inn;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    bool CEmployee::getFailed() const
    {
        return m_failed;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    string CEmployee::getOrganisation() const
    {
        return m_organisation;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    QDate CEmployee::getHireDate() const
    {
        return m_hireDate;
    }

    // ---------- ---------- ---------- ---------- ---------- ----------
    QDate CEmployee::getDismissionDate () const
    {
        return m_dismissionDate;
    }
}
