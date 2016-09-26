#include <QString>
#include <QtTest>
#include <segment.h>
#include <vector>

class TestFragmentsTest : public QObject
{
    Q_OBJECT

public:
    TestFragmentsTest();

private Q_SLOTS:
    void testBuildFragmentObjects_data();
    void testBuildFragmentObjects();

    void testMergeFragmentObjects_data();
    void testMergeFragmentObjects();

};

TestFragmentsTest::TestFragmentsTest()
{
}

Q_DECLARE_METATYPE(const char*)
void TestFragmentsTest::testBuildFragmentObjects_data()
{
    QTest::addColumn<QByteArray>("data");
    QTest::addColumn<quint64>("address");
    QTest::addColumn<QString>("header");

    QTest::newRow("Fragment1") << QByteArray("hello") << (quint64)0x80000000<< QString("header");
    QTest::newRow("Fragment empty data") << QByteArray("") << (quint64)0x0000000<< QString("header2");

}

void TestFragmentsTest::testBuildFragmentObjects()
{
    QFETCH(QByteArray, data);
    QFETCH(quint64, address);
    QFETCH(QString, header);
    Segment test1(data.constData(),data.size(),address,header);
    QCOMPARE(test1.address(), address);
    QCOMPARE(test1.header(),header);
    QCOMPARE(test1.size(),(quint64)data.size());
}

Q_DECLARE_METATYPE(Segment)

void TestFragmentsTest::testMergeFragmentObjects_data()
{
    QTest::addColumn<Segment>("fragment1");
    QTest::addColumn<Segment>("fragment2");
    QTest::addColumn<bool>("result");

    QTest::newRow("Mergeable")
            << Segment("hello",5,0,"header")
            << Segment("hello",5,5,"header")<< true;
    QTest::newRow("Not Mergeable")
            << Segment("hello",5,0,"header")
            << Segment("hello World",11,32,"header")<< false;
}

void TestFragmentsTest::testMergeFragmentObjects()
{
    QFETCH(Segment, fragment1);
    QFETCH(Segment, fragment2);
    QFETCH(bool, result);
    QCOMPARE(fragment1.merge(fragment2),result);
}

QTEST_APPLESS_MAIN(TestFragmentsTest)

#include "tst_testfragmentstest.moc"
