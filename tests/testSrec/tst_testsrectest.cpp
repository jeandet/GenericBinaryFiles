/*------------------------------------------------------------------------------
--  This file is a part of the GenericBinaryFiles library
--  Copyright (C) 2016, Plasma Physics Laboratory - CNRS
--
--  This program is free software; you can redistribute it and/or modify
--  it under the terms of the GNU General Public License as published by
--  the Free Software Foundation; either version 2 of the License, or
--  (at your option) any later version.
--
--  This program is distributed in the hope that it will be useful,
--  but WITHOUT ANY WARRANTY; without even the implied warranty of
--  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
--  GNU General Public License for more details.
--
--  You should have received a copy of the GNU General Public License
--  along with this program; if not, write to the Free Software
--  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
-------------------------------------------------------------------------------*/
/*--                  Author : Alexis Jeandet
--                     Mail : alexis.jeandet@member.fsf.org
----------------------------------------------------------------------------*/
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
