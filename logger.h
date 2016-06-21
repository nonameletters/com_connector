#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <fstream>
#include <string>

#include <QString>
#include <QByteArray>

// #include <person.h>

using namespace std;
// using namespace db_connector;

#define LL CLogger::getInstance()
#define OLL (*CLogger::getInstance())

namespace ns_logger
{
    class CLogger
    {
        public:
            static CLogger* getInstance();
            void log(const char* mess);
            void log(const string& str);
            void log(const QString& str);
            void log(const QByteArray& str);
            void close();
            ~CLogger();


            CLogger* operator<<(const char* str);
//            friend CLogger* operator<<(CLogger* l, const char* str);
            friend CLogger* operator<<(CLogger* l, const string& str);
            friend CLogger* operator<<(CLogger* l, const QString& str);
            friend CLogger* operator<<(CLogger* l, const QByteArray& str);
//            friend CLogger* operator<<(CLogger* l, const CPerson& p);

        private:
            CLogger();

        private:
            static CLogger* m_instance;
            ofstream m_log;
    };
}
#endif // LOGGER_H
