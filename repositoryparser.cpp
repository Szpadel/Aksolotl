#include "repositoryparser.h"

RepositoryParser::RepositoryParser(QObject *parent) :
    QObject(parent)
{
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

bool RepositoryParser::addValidScheme(QString scheme)
{
    if(!validSchemes.contains(scheme))
    {
        validSchemes.push_back(scheme);
    };
    return true;
}

bool RepositoryParser::addValidSchemes(QList<QString> schemes)
{
    foreach (QString scheme, schemes) {
        addValidScheme(scheme);
    }
    return true;
}

bool RepositoryParser::removeValidScheme(QString scheme)
{
    return validSchemes.removeOne(scheme);
}

bool RepositoryParser::removeValidSchemes(QList<QString> schemes)
{
    foreach (QString scheme, schemes) {
        removeValidScheme(scheme);
    }
    return true;
}

bool RepositoryParser::flushValidSchemes()
{
    return validSchemes.empty();
}
