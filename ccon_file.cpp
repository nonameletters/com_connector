#include "ccon_file.h"

// ---------- ---------- ---------- ---------- ---------- ----------
CConFile::CConFile()
{
    QTextCodec* l_codec = QTextCodec::codecForName("CP866");

    QStringList l_nameFilter;
    l_nameFilter.append("*.xml");

    m_dir = make_shared<QDir>("\\\\srv-app\\anviz_sync\\users\\");
    m_dir->setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    m_dir->setNameFilters(l_nameFilter);

    QFileInfoList l_fList = m_dir->entryInfoList();

    m_files.clear();
    auto it = begin(l_fList);
    for(; it != end(l_fList); it++)
    {
        cout << l_codec->fromUnicode((*it).fileName()).data() << endl;
        cout << l_codec->fromUnicode((*it).absoluteFilePath()).data() << endl;
        m_files.append((*it).absoluteFilePath());
    }
}

// ---------- ---------- ---------- ---------- ---------- ----------
vector<CPerson*> CConFile::getPersonList(Filter flag) const
{
    vector<CPerson*> l_list;

    QXmlSimpleReader l_reader;

    CSXmlHandler l_handler;

    l_reader.setContentHandler(&l_handler);
    l_reader.setErrorHandler(&l_handler);

    auto it = begin(m_files);
    for (; it != end(m_files); it++)
    {
        QFile l_file(*it);
        QXmlInputSource l_source(&l_file);
        l_reader.parse(l_source);

    }

    if (flag == ALL)
    {
        return l_list;
    }

    return l_list;
}

// ---------- ---------- ---------- ---------- ---------- ----------
// Additional class to react on reader events
// ---------- ---------- ---------- ---------- ---------- ----------
CSXmlHandler::CSXmlHandler()
{

}

// ---------- ---------- ---------- ---------- ---------- ----------
bool CSXmlHandler::startElement(const QString &namespaceURI, const QString &localName,
              const QString &qName, const QXmlAttributes &attributes)
{

    cout << "startElement" << endl;
    cout << "namespaceUri: " << namespaceURI.toStdString().c_str() << endl;
    cout << "localName: " << localName.toStdString().c_str() << endl;
    cout << "qName: " << qName.toStdString().c_str() << endl;
    cout << "value: " << attributes.value(qName).toStdString() << " ." << endl;

    return true;
}

// ---------- ---------- ---------- ---------- ---------- ----------
bool CSXmlHandler::endElement(const QString &namespaceURI, const QString &localName,
            const QString &qName)
{
    return true;
}

// ---------- ---------- ---------- ---------- ---------- ----------
bool CSXmlHandler::characters(const QString &str)
{
    cout << "Chunk value: " << str.toStdString() << endl;
    return true;
}

// ---------- ---------- ---------- ---------- ---------- ----------
bool CSXmlHandler::fatalError(const QXmlParseException &exception)
{
    cout << "fatalError" << endl;
    return true;
}

// ---------- ---------- ---------- ---------- ---------- ----------
QString CSXmlHandler::errorString() const
{
    return QString("Unknown error");
}

// ---------- ---------- ---------- ---------- ---------- ----------
CPerson CSXmlHandler::getPerson() const
{
    return m_person;
}
