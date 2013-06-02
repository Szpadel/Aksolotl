#ifndef REPOSITORYPARSER_H
#define REPOSITORYPARSER_H

#include <QObject>
#include <QStringList>
#include <QUrl>
#include <QDebug>

class RepositoryParser : public QObject
{
    Q_OBJECT
public:
    explicit RepositoryParser(QObject *parent = 0);

    /**
     * @brief parseRepository metoda parsuje podany QString po enterach i usuwa /r, a następnie zwraca listę QUrl
     * @param repository QString downloaded by ReutDownloader
     * @return QList<QUrl> Qlist of QUrls
     */
    QList<QUrl> parseRepositories(QString repository);
private:
    bool validateRepoUrl(const QUrl &url);
    QList<QString> validSchemes;
signals:
    
public slots:
    
};

#endif // REPOSITORYPARSER_H
