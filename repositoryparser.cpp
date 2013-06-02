#include "repositoryparser.h"

RepositoryParser::RepositoryParser(QObject *parent) :
    QObject(parent)
{
    validSchemes.push_back("http");
    validSchemes.push_back("https");
    validSchemes.push_back("ftp");


}

QList<QUrl> RepositoryParser::parseRepositories(QString repository)
{
    QStringList temp;
    temp = repository.replace("\r","").split("\n",QString::SkipEmptyParts);

    QList<QUrl> tempLinksList;
    foreach (QString linkString, temp) {

        QUrl tempLink = QUrl::fromUserInput(linkString);

        if(validateRepoUrl(tempLink))
        {
            tempLinksList.push_back(tempLink);
        }
    }
    return tempLinksList;
}

bool RepositoryParser::validateRepoUrl(const QUrl &url)
{

    bool isValid = url.isValid();
    if(!validSchemes.isEmpty()) isValid = validSchemes.contains(url.scheme())?true:false;

    return isValid;
}

