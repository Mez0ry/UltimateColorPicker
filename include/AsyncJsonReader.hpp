#ifndef ASYNCJSONREADER_HPP
#define ASYNCJSONREADER_HPP
#include <QCoreApplication>
#include <QFile>
#include <QByteArray>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QDebug>

class AsyncJsonReader : public QObject {
    Q_OBJECT
private:
    QFile m_File;
    QJsonParseError m_Error;
    int m_MaxChunksPerDoc = 1;
    int m_ProccessedChunksCount = 0;
    QByteArray m_AccumulatedData;
public:
    explicit AsyncJsonReader(QObject *parent = nullptr);

    bool ReadFromFile(const QString &file_path, int max_chunks_per_doc = 1);
signals:
    void RequestNextChunk();
    void ProcessedDocument(const QJsonDocument&);
    void FinishedReading(const QJsonDocument&);
private slots:
    void OnRequestNextChunk();
private:
    void parseBuffer(const QByteArray buffer);
};


#endif // ASYNCJSONREADER_HPP
