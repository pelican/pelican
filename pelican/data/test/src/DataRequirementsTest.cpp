/*
 * Copyright (c) 2013, The University of Oxford
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of the University of Oxford nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "data/test/DataRequirementsTest.h"
#include "data/DataRequirements.h"
#include <QtCore/QSet>
#include <QtCore/QStringList>

namespace pelican {

CPPUNIT_TEST_SUITE_REGISTRATION( DataRequirementsTest );
// class DataRequirementsTest
DataRequirementsTest::DataRequirementsTest()
    : CppUnit::TestFixture()
{
}

DataRequirementsTest::~DataRequirementsTest()
{
}

void DataRequirementsTest::setUp()
{
}

void DataRequirementsTest::tearDown()
{
}

void DataRequirementsTest::test_hash()
{
    // Use Case:
    // Ensure hash returns a non-zero value.
    {
        DataRequirements d;
        d.addRequired(QString("hello"));
        CPPUNIT_ASSERT(d.hash() != 0);
    }

    // Use Case:
    // Ensure different DataRequirement objects return different hash values.
    {
        DataRequirements d1;
        d1.addRequired(QString("one"));
        DataRequirements d2;
        d2.addRequired(QString("two"));
        CPPUNIT_ASSERT(d1.hash() != d2.hash());
    }

    // Use Case:
    // Ensure different ordering returns the same hash.
    {
        DataRequirements d1;
        d1.addRequired("one");
        d1.addRequired("two");

        DataRequirements d2;
        d2.addRequired("two");
        d2.addRequired("one");
        CPPUNIT_ASSERT(d1.hash() == d2.hash());
    }
}

void DataRequirementsTest::test_operator_equalTo()
{
    // Use Case:
    // Test for identical objects.
    {
        DataRequirements d1;
        d1.addRequired(QString("hello"));
        DataRequirements d2;
        d2.addRequired(QString("hello"));
        CPPUNIT_ASSERT(d1 == d2);
    }
}

void DataRequirementsTest::test_operator_notEqualTo()
{
    // Use Case:
    // Test for non identical objects.
    {
        DataRequirements d1;
        d1.addRequired(QString("hello1"));
        DataRequirements d2;
        d2.addRequired(QString("hello2"));
        CPPUNIT_ASSERT(d1 != d2);
    }
}

void DataRequirementsTest::test_operator_additiveAssignment()
{
    // Use Case:
    // Test for merging two requirements objects.
    {
        DataRequirements d1, d2, d3;
        QStringList req;
        req << "one" << "two";

        d1.addRequired(req.at(0));
        d2.addRequired(req.at(1));
        d3.addRequired(req.toSet());
        d1 += d2;
        CPPUNIT_ASSERT(d1 == d3);
    }

    // Use Case:
    // Test for merging two requirements objects in a different order.
    {
        DataRequirements d1, d2, d3;
        QStringList req;
        req << "one" << "two";

        d1.addRequired(req.at(0));
        d2.addRequired(req.at(1));
        d3.addRequired(req.toSet());
        d2 += d1;
        CPPUNIT_ASSERT(d2 == d3);
    }

    // Use Case:
    // Test for merging two requirements objects, removing duplicates.
    {
        DataRequirements d, d1, d2;
        QSet<QString> req, req1, req2;
        req  << "one" << "two" << "three";
        req1 << "one" << "two";
        req2 << "two" << "three";

        d.addRequired(req);
        d1.addRequired(req1);
        d2.addRequired(req2);
        d2 += d1;
        CPPUNIT_ASSERT(d == d2);
    }
}

} // namespace pelican
