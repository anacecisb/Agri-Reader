#ifndef QCRESULTSMANAGER_H
#define QCRESULTSMANAGER_H

#include <QObject>
#include <qtreewidget.h>

class QCResultsManager : public QObject
{
    Q_OBJECT
public:
    explicit QCResultsManager(QObject *parent = 0);

    QString generateAnalysisResultsFromTree( QTreeWidget* tree );

    void saveResultStringToFile( QString resultString, QString filename );
    QString loadResultFile( QString filename );

    void loadAllResults();
    QStringList allResults;
//    QString analysisResultString;
signals:

public slots:

};

#endif // QCRESULTSMANAGER_H
