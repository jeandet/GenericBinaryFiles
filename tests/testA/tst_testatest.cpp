#include <QString>
#include <QtTest>
#include <binaryfile.h>
#include <iostream>
#include <srec/srecfileio.h>
#include <private/binaryfiledataprivate.h>

class TestATest : public QObject
{
    Q_OBJECT

public:
    TestATest();

private Q_SLOTS:
    void testCase1();
};

TestATest::TestATest()
{
    size_t sz =1024;
    char* a = new char[sz];
   Segment f(a,sz,1234);
    std::cout << f.address() << f.header().toStdString() << f.size() << std::endl;
    SrecFileIO test;
    test.read(QFINDTESTDATA("./sample.srec"));
}

void TestATest::testCase1()
{
    QVERIFY2(true, "Failure");
}

QTEST_APPLESS_MAIN(TestATest)

#include "tst_testatest.moc"
