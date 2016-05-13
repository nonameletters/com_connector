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
        if (l_reader.parse(l_source))
        {
            cout << "// Parse OK ---------- ---------- ---------- ---------- ---------- ----------" << endl;
            l_list.push_back(new CPerson(l_handler.getPerson()));
        }

    }

    auto it1 = begin(l_list);
    for(;it1 != end(l_list); it1++)
    {
        cout << (*(*it1));
    }

    cout << "Persons was readerd: " << l_list.size() << endl;
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

    m_curNode = qName;

    // TO REMOVE WARNING
    gag(namespaceURI);
    gag(localName);
    attributes.length();

    // cout << "startElement" << endl;
    // cout << "namespaceUri: " << namespaceURI.toStdString().c_str() << endl;
    // cout << "localName: " << localName.toStdString().c_str() << endl;
    // cout << "qName: " << qName.toStdString().c_str() << endl;

    return true;
}

// ---------- ---------- ---------- ---------- ---------- ----------
bool CSXmlHandler::endElement(const QString &namespaceURI, const QString &localName,
            const QString &qName)
{
    // TO REMOVE WARNING
    gag(namespaceURI);
    gag(localName);
    gag(qName);

    m_curNode = "unknown";
    return true;
}

// ---------- ---------- ---------- ---------- ---------- ----------
bool CSXmlHandler::characters(const QString &str)
{
//    QTextCodec* l_c = QTextCodec::codecForName("CP866");

    if (m_curNode == "synced")
    {
        m_person.setActivity(str.toInt());
        //cout << "Chunk value: " << l_c->fromUnicode(str).data() << endl;
        return true;
    }
    else if (m_curNode == "db_id")
    {
        m_person.setGroupSid(str);
        //cout << "Chunk value: " << l_c->fromUnicode(str).data() << endl;
        return true;
    }
    else if (m_curNode == "familiya")
    {
        m_person.setFirstName(str);
        //cout << "Chunk value: " << l_c->fromUnicode(str).data() << endl;
        return true;
    }
    else if (m_curNode == "imya")
    {
        m_person.setMidleName(str);
        //cout << "Chunk value: " << l_c->fromUnicode(str).data() << endl;
        return true;
    }
    else if (m_curNode == "otchestvo")
    {
        m_person.setLastName(str);
        //cout << "Chunk value: " << l_c->fromUnicode(str).data() << endl;
        return true;
    }
    else if (m_curNode == "dogovor")
    {
        m_person.setVidDogovora(str);
        //cout << "Chunk value: " << l_c->fromUnicode(str).data() << endl;
        return true;
    }
    else if (m_curNode == "zanyatost")
    {
        m_person.setVidZanyatosti(str);
        //cout << "Chunk value: " << l_c->fromUnicode(str).data() << endl;
        return true;
    }
    else if (m_curNode == "birthday")
    {
        m_person.setBirthDay(QDate::fromString(str, "dd-MM-yyyy"));
        //cout << "Chunk value: " << l_c->fromUnicode(str).data() << endl;
        return true;
    }
    else if (m_curNode == "tab_number")
    {
        m_person.setTableNumber(str);
        //cout << "Chunk value: " << l_c->fromUnicode(str).data() << endl;
        return true;
    }
    else if (m_curNode == "snils")
    {
        m_person.setSnils(str);
        //cout << "Chunk value: " << l_c->fromUnicode(str).data() << endl;
        return true;
    }
    else if (m_curNode == "comment")
    {
        m_person.setComment(str);
        //cout << "Chunk value: " << l_c->fromUnicode(str).data() << endl;
        return true;
    }
    else if (m_curNode == "dolgnost")
    {
        m_person.setPosition(str.toStdString());
        //cout << "Chunk value: " << l_c->fromUnicode(str).data() << endl;
        return true;
    }
    else
    {
        return true;
        //cout << "Unknown node: " << m_curNode.toStdString() << endl;
    }

    return true;
}

// ---------- ---------- ---------- ---------- ---------- ----------
bool CSXmlHandler::fatalError(const QXmlParseException &exception)
{
    cout << "Parse error: " << exception.message().toStdString() << endl;
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
