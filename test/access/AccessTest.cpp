#include <QtTest>
#include <Serializer.h>
#include <model_pch.h>

struct MockDataStreamSerializable : public QDataStreamSerializable
{
    quint32 unsignedInt32;
    qint32 signedInt32;
    QString string;
    QPair<qint32, QString> intStringPair;
    QVector<quint32> vectorOfUnsignedInt32;

    virtual QDataStream& serialize(QDataStream& s) const override
    {
        s << unsignedInt32 << signedInt32 << string << intStringPair << vectorOfUnsignedInt32;
        return s;
    }

    virtual QDataStream& deserialize(QDataStream& s) override
    {
        s >> unsignedInt32 >> signedInt32 >> string >> intStringPair >> vectorOfUnsignedInt32;
        return s;
    }

    bool operator==(const MockDataStreamSerializable& other) const
    {
        return std::tie(unsignedInt32, signedInt32, string, intStringPair, vectorOfUnsignedInt32)
            == std::tie(other.unsignedInt32, other.signedInt32, other.string, other.intStringPair, other.vectorOfUnsignedInt32);
    }
};

class SerializerTest : public QObject
{
    Q_OBJECT
public:
    SerializerTest() = default;

    static const QString testFileName1;
    static const QString testFileName2;

    static void cleanDirectory(QString& dirName);

private slots:
    void init();
    void initTestCase();
    void cleanupTestCase();
    void testGenerateFileName();
    void testSerialize();
    void testDeserialize();
    void testGetAvailableFiles();
private:
    Serializer mSerializer;
};

const QString SerializerTest::testFileName1{"saveFirst.bin"};
const QString SerializerTest::testFileName2{"saveSecond.bin"};

void SerializerTest::cleanDirectory(QString &dirName)
{
    QDir workingDirectory(dirName);
    if(workingDirectory.exists())
    {
        workingDirectory.removeRecursively();
    }
}

void SerializerTest::init()
{
    QString dir = "data";
    cleanDirectory(dir);
    mSerializer = Serializer();
}

void SerializerTest::initTestCase()
{
}
void SerializerTest::cleanupTestCase()
{
}
void SerializerTest::testGenerateFileName()
{
    QString fileName = mSerializer.generateFileName();
}

void SerializerTest::testSerialize()
{
    QString fileName = mSerializer.generateFileName();
    MockDataStreamSerializable mockSerializable;
    MockDataStreamSerializable serializationCopy(mockSerializable);
    mSerializer.serialize(mockSerializable, fileName);

    QCOMPARE(mockSerializable, serializationCopy);
}
void SerializerTest::testDeserialize()
{
    QString fileName = mSerializer.generateFileName();
    MockDataStreamSerializable mockSerializable;

    size_t mockSize = sizeof (MockDataStreamSerializable);

    mockSerializable.string = "Hello from the other side";
    mockSerializable.unsignedInt32 = 412;
    mockSerializable.vectorOfUnsignedInt32 = {3, 4, 1, 242, 145};

    mSerializer.serialize(mockSerializable, fileName);
    MockDataStreamSerializable deserialized;
    mSerializer.deserialize(deserialized, fileName);

    QCOMPARE(deserialized, mockSerializable);
}
void SerializerTest::testGetAvailableFiles()
{
    MockDataStreamSerializable mockSerializable1;
    MockDataStreamSerializable mockSerializable2;
    MockDataStreamSerializable mockSerializable3;
    MockDataStreamSerializable mockSerializable4;

    mSerializer.serialize(mockSerializable1, mSerializer.generateFileName());
    mSerializer.serialize(mockSerializable2, mSerializer.generateFileName());
    mSerializer.serialize(mockSerializable3, mSerializer.generateFileName());
    mSerializer.serialize(mockSerializable4, mSerializer.generateFileName());

    QStringList availableFiles = mSerializer.availableFiles();
    QVERIFY(!availableFiles.empty());
    QCOMPARE(availableFiles.size(), 4);
}
QTEST_APPLESS_MAIN(SerializerTest);

#include "AccessTest.moc"
