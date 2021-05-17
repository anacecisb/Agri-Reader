#include "qcresultsmanager.h"

#include "qfile.h"
#include <QDirIterator>
#include "qtextstream.h"
#include <QTime>
#include <QDebug>

#include "IVDCore/ivdtimehelper.h"

QCResultsManager::QCResultsManager(QObject *parent) :
    QObject(parent)
{
}


QString QCResultsManager::generateAnalysisResultsFromTree( QTreeWidget* tree )
{
    QString resultString;
    QStringList resultStringList;

    resultStringList << "Date:" << IVDTimeHelper::getDateYMDFormatA() << " Time:" << IVDTimeHelper::getTimeHMSFormatA() << "\n\n";

    for ( int x = 0; x < tree->topLevelItemCount(); x++ )
//    foreach( QWidget* tChild, tree->chi )
    {
//        QTreeWidgetItem* roiSystemItem = (QTreeWidgetItem*)tChild;
        QTreeWidgetItem* roiSystemItem = tree->topLevelItem(x);
        QString ROISystemName = roiSystemItem->text(0);
        resultStringList << ROISystemName << "\n";

        for ( int i = 0; i < roiSystemItem->childCount(); i++ )
        {
            QTreeWidgetItem* ROI = roiSystemItem->child(i);
            QString ROIResult = ROI->text(0);
            resultStringList << "\t" << ROIResult << "\n";
        }
        resultStringList << "\n";
    }

    resultString = resultStringList.join("");

    return resultString;
}

void QCResultsManager::saveResultStringToFile( QString resultString, QString filename )
{
    QFile file( filename );
    if ( !file.open( QIODevice::WriteOnly | QIODevice::Text ) )
    {
         qDebug() << "ERROR: saveResultStringToFile opening results file[" << filename << "] " << file.errorString();
         return;
    }

    file.flush();
    QTextStream out(&file);

    out << resultString;

    file.close();
}


QString QCResultsManager::loadResultFile(QString filename)
{
    QString filepath = QString( "assay_results/%1" ).arg(filename);
    QFile file( filepath );
    if ( !file.open( QIODevice::ReadOnly | QIODevice::Text ) )
    {
         qDebug() << "ERROR: loadResultFile opening results file[" << filepath << "] " << file.errorString();
    }
    else
    {
        QTextStream in(&file);

        QString resultFile = in.readAll();

        file.close();
//            qDebug() << "loaded " << filename << "_" << resultFile;
       return resultFile;
    }

    return QString();
}



void QCResultsManager::loadAllResults()
{
    allResults.clear();

    QDir dir("assay_results" );
    QStringList resultFiles = dir.entryList();

    foreach( QString filename, resultFiles )
    {
        qDebug() << filename;

        QString filepath = QString( "assay_results/%1" ).arg(filename);
        QFile file( filepath );
        if ( !file.open( QIODevice::ReadOnly | QIODevice::Text ) )
        {
             qDebug() << "ERROR: loadAllResults opening results file[" << filepath << "] " << file.errorString();
        }
        else
        {
            QTextStream in(&file);

            QString resultFile = in.readAll();

            file.close();
//            qDebug() << "loaded " << filename << "_" << resultFile;
            allResults.push_back( resultFile );
        }
    }
}
