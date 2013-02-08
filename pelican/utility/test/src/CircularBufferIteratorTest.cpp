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

#include "CircularBufferIteratorTest.h"
#include "CircularBufferIterator.hpp"


namespace pelican {

CPPUNIT_TEST_SUITE_REGISTRATION( CircularBufferIteratorTest );
/**
 *@details CircularBufferIteratorTest
 */
CircularBufferIteratorTest::CircularBufferIteratorTest()
    : CppUnit::TestFixture()
{
}

/**
 *@details
 */
CircularBufferIteratorTest::~CircularBufferIteratorTest()
{
}

void CircularBufferIteratorTest::setUp()
{
}

void CircularBufferIteratorTest::tearDown()
{
}

void CircularBufferIteratorTest::test_circular()
{
     { // Use Case:
       // buffer size > 1
       // Expect:
       // next to return a different object for each call
       // until it reaches n, then recycle them in the same order
       QList<int> buf; buf << 1 << 2 << 3;
       pelican::CircularBufferIterator<int> it(&buf);
       CPPUNIT_ASSERT_EQUAL(1, it.next());
       CPPUNIT_ASSERT_EQUAL(1, *it);
       CPPUNIT_ASSERT_EQUAL(2, it.peekNext());
       CPPUNIT_ASSERT_EQUAL(2, it.next());
       CPPUNIT_ASSERT_EQUAL(2, *it);
       CPPUNIT_ASSERT_EQUAL(3, it.next());
       CPPUNIT_ASSERT_EQUAL(1, it.peekNext());
       CPPUNIT_ASSERT_EQUAL(1, it.next());
       CPPUNIT_ASSERT_EQUAL(2, it.next());
       CPPUNIT_ASSERT_EQUAL(2, *it);
       CPPUNIT_ASSERT_EQUAL(3, it.next());
       CPPUNIT_ASSERT_EQUAL(1, it.next());
       CPPUNIT_ASSERT_EQUAL(2, it.next());
       CPPUNIT_ASSERT_EQUAL(2, *it);
       CPPUNIT_ASSERT_EQUAL(3, it.next());
       // test resetIndex - existing value
       it.resetIndex(2);
       CPPUNIT_ASSERT_EQUAL(2, *it);
       CPPUNIT_ASSERT_EQUAL(3, it.next());
       // test resetIndex - non-existing value
       // expect to reset to beginning
       it.resetIndex(6);
       CPPUNIT_ASSERT_EQUAL(1, it.next());
     }
}

} // namespace pelican
