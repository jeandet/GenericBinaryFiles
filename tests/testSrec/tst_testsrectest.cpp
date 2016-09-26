#include <QString>
#include <QtTest>
#include <iostream>
#include <srec/srecfileio.h>
#include <binaryfile.h>
#include <private/binaryfiledataprivate.h>

class TestSrecTest : public QObject
{
    Q_OBJECT

public:
    TestSrecTest();

private Q_SLOTS:
    void testCaseNominal();
    void testCaseWrongCRC();
    void testCaseWrongRecordType();
};

TestSrecTest::TestSrecTest()
{
}

void TestSrecTest::testCaseNominal()
{
    BinaryFile file;
    file.load(QFINDTESTDATA("./sample.srec"),"srec");
    QVERIFY(file.segments().count()==6);
}

void TestSrecTest::testCaseWrongCRC()
{
    SrecFileIO test;
    QVERIFY_EXCEPTION_THROWN(test.read(QFINDTESTDATA("./WrongCRC.srec")),std::runtime_error);
}

void TestSrecTest::testCaseWrongRecordType()
{
    SrecFileIO test;
    QVERIFY_EXCEPTION_THROWN(test.read(QFINDTESTDATA("./WrongRecordType.srec")),std::runtime_error);
}

QTEST_APPLESS_MAIN(TestSrecTest)

#include "tst_testsrectest.moc"
