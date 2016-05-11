#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include <string>
#include <QDate>

using namespace std;

namespace nsComConnector
{
    class CEmployee
    {
        public:
            CEmployee();

            void setName(string name);
            void setNumber(string number);
            void setPosition(string position);
            void setInn(string inn);
            void setFailed(bool failed);
            void setOrganisation(string org);
            void setHireDate(QDate date);
            void setDismissionDate(QDate date);

            string getName()     const;
            string getNumber()   const;
            string getPosition() const;
            string getInn()      const;
            bool   getFailed()   const;
            string getOrganisation() const;
            QDate  getHireDate() const;
            QDate  getDismissionDate () const;

        private:
            string m_name;
            string m_number;
            string m_position;
            string m_inn;
            bool   m_failed;
            string m_organisation;
            QDate  m_hireDate;
            QDate  m_dismissionDate;
    };
}
#endif // EMPLOYEE_H
