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

    /**
     * @brief getValidSchemes zwraca dozwolne protokoly
     * @return QList<QString> z dozwolonymi protokolamy (np. http)
     */
    QList<QString> getValidSchemes();

    /**
     * @brief addValidScheme dodaje protokol do listy dozwolonych
     * @param scheme QString; Dozwolony protokol (np. http)
     * @return bool; false - blad dodawania, true - dodano poprawnie, lub protokol juz istnieje;
     */
    bool addValidScheme(QString scheme);

    /**
     * @brief addValidSchemes dodaje liste protokolow do listy dozwolonych
     * @return bool; false - blad dodawania, true - dodano poprawnie, lub protokoly juz istnieja;
     */
    bool addValidSchemes(QList<QString> schemes);

    /**
     * @brief removeValidScheme usuwa protokol z listy dozwolonych protokolow;
     * @param scheme QString zawierajacy protokol (np. http)
     * @return bool; false - blad usuwania, true - usunieto protokol, lub protokolu juz nie ma na liscie;
     */
    bool removeValidScheme(QString scheme);

    /**
     * @brief removeValidSchemes
     * @param QList<QString> zawierajacy protokoly do dodania
     * @return int; 0 - blad usuwania, 1 - usunieto protokoly, lub protkolow juz nie ma na liscie;
     */
    bool removeValidSchemes(QList<QString> schemes);

    /**
     * @brief flushValidSchemes usuwa dozwolone protokoly z listy (akceptuje wszystkie protokoly)
     * @return int; 0 - blad usuwania; 1 - lista wyczyszczona;
     */
    bool flushValidSchemes();
private:
    bool validateRepoUrl(const QUrl &url);
    QList<QString> validSchemes;
signals:
    
public slots:
    
};

#endif // REPOSITORYPARSER_H
