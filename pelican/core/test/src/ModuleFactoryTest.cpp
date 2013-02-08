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

#include "ModuleFactoryTest.h"
#include "modules/AbstractModule.h"
#include "utility/FactoryConfig.h"
#include "utility/Config.h"
#include <iostream>

namespace pelican {

CPPUNIT_TEST_SUITE_REGISTRATION( ModuleFactoryTest );

/**
 * @details
 * ModuleFactoryTest constructor.
 */
ModuleFactoryTest::ModuleFactoryTest()
    : CppUnit::TestFixture()
{
}

/**
 * @details
 * ModuleFactoryTest destructor.
 */
ModuleFactoryTest::~ModuleFactoryTest()
{
}

/**
 * @details
 * Set-up routine called before each test.
 */
void ModuleFactoryTest::setUp()
{
}

/**
 * @details
 * Clean-up routine called after each test.
 */
void ModuleFactoryTest::tearDown()
{
}

/**
 * @details
 * Tries to create an EmptyModule with and without a configuration object.
 */
void ModuleFactoryTest::test_createModule_EmptyModule()
{
    // Use Case:
    // Ask for a module without any configuration
    // Expect to be given a basic object
    {
        Config emptyConfig;
        FactoryConfig<AbstractModule> factory(&emptyConfig, "", "");
        AbstractModule* module = 0;
        CPPUNIT_ASSERT_NO_THROW(module = factory.create("EmptyModule"));
        CPPUNIT_ASSERT(module != 0);
    }

    // Use Case:
    // Ask for a module with a configuration
    {
        Config config;
        QString section("pipeline"), group("modules");
        Config::TreeAddress base;
        base.append(Config::NodeId(section, ""));
        base.append(Config::NodeId(group, ""));
        Config::TreeAddress address(base);
        address.append(Config::NodeId("EmptyModule", ""));
        config.set(address);
        FactoryConfig<AbstractModule> factory(&config, section, group);

        AbstractModule* module = 0;
        CPPUNIT_ASSERT_NO_THROW(module = factory.create("EmptyModule"));
        CPPUNIT_ASSERT(module != 0);
    }
}

} // namespace pelican
