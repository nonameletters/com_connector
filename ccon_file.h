#ifndef CCON_FILE_H
#define CCON_FILE_H

#include <QDir>
#include <QFileInfoList>
#include <QStringList>
#include <QTextCodec>
#include <QXmlSimpleReader>
#include <QXmlInputSource>
#include <QXmlDefaultHandler>
#include <QDate>
#include <memory>

#include <iostream>
#include <vector>
#include <algorithm>

#include "person.h"

using namespace std;
using namespace db_connector;


class CConFile
{
    public:
        enum Filter
        {
            ALL,
            NEW
        };

    public:
        CConFile();

        vector<CPerson*> getPersonList(Filter flag = ALL) const;

    private:
        shared_ptr<QDir> m_dir;
        QStringList m_files;
};


class CSXmlHandler : public QXmlDefaultHandler
{
    public:
        CSXmlHandler();

        bool startElement(const QString &namespaceURI, const QString &localName,
                          const QString &qName, const QXmlAttributes &attributes);
        bool endElement(const QString &namespaceURI, const QString &localName,
                        const QString &qName);
        bool characters(const QString &str);
        bool fatalError(const QXmlParseException &exception);
        QString errorString() const;

        CPerson getPerson() const;

        template<typename T>
        void gag(T param)
        {
            if (param == param)
            {
                return;
            }
        }

    private:
        CPerson m_person;
        QString m_curNode = "";
};
#endif // CCON_FILE_H
