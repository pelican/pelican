#include "ImageWriterFitsTest.h"
#include "modules/ImageWriterFits.h"
#include "utility/Config.h"
#include "data/ImageData.h"
#include <iostream>

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
    QDomElement config;
    ImageWriterFits fits(config);
}


void ImageWriterFitsTest::test_openFile()
{
    QDomElement config;
    ImageWriterFits fits(config);

    ImageData image(128, 128, 512, 2);
    fits._image = &image;

    fits.fileName() = "test";
    fits._open();
    fits._writeHeader();
    fits._writeImage();
    fits._close();
}


} // namespace pelican
