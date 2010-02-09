#include "ImageWriterFitsTest.h"
#include "ImageWriterFits.h"
#include "utility/Config.h"
#include "utility/memCheck.h"

namespace pelican {

CPPUNIT_TEST_SUITE_REGISTRATION( ImageWriterFitsTest );

// class ImageWriterFitsTest
ImageWriterFitsTest::ImageWriterFitsTest()
    : CppUnit::TestFixture()
{
}

ImageWriterFitsTest::~ImageWriterFitsTest()
{
}

void ImageWriterFitsTest::setUp()
{
}

void ImageWriterFitsTest::tearDown()
{
}

void ImageWriterFitsTest::test_createModule()
{
//    Config config;
    QDomElement config;
    ImageWriterFits fitsWriter(config);

}

} // namespace pelican
