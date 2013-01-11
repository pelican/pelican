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

#include "utility/test/ConfigNodeTest.h"
#include "utility/ConfigNode.h"

#include <QtCore/QCoreApplication>

namespace pelican {

CPPUNIT_TEST_SUITE_REGISTRATION( ConfigNodeTest );
// class ConfigTest
ConfigNodeTest::ConfigNodeTest()
    : CppUnit::TestFixture()
{
}

ConfigNodeTest::~ConfigNodeTest()
{
}

void ConfigNodeTest::setUp()
{
    int ac = 0;
    app = new QCoreApplication(ac, NULL);
}

void ConfigNodeTest::tearDown()
{
    delete app;
}

/**
 * @details
 * Test for empty configuration file name string.
 */
void ConfigNodeTest::test_methods()
{

}


/**
 * @details
 */
void ConfigNodeTest::test_setFromString()
{
    QString xml = ""
            "<node>"
                "<property1 attribute=\"value1\"/>"
                "<property2 attribute=\"value2\"/>"
            "</node>";
    ConfigNode node;
    node.setFromString(xml);
    CPPUNIT_ASSERT(node.getOption("property1", "attribute", "") == QString("value1"));
    CPPUNIT_ASSERT(node.getOption("property2", "attribute", "") == QString("value2"));
}




} // namespace pelican
