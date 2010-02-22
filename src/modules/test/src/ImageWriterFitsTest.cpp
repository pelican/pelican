#include "ImageWriterFitsTest.h"
#include "modules/ImageWriterFits.h"
#include "utility/Config.h"
#include "data/ImageData.h"
#include <QCoreApplication>
#include <QFile>
#include "utility/pelicanTimer.h"
#include "data/DataBlob.h"
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
    int ac = 0;
    _app = new QCoreApplication(ac, NULL);
}

void ImageWriterFitsTest::tearDown()
{
//    QFile file("test.fits");
//    if (file.exists()) file.remove();
    delete _app;
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
    real_t* im = image.ptr();
//    fits._writeImage(im, 128, 128);
    fits._close();
}

void ImageWriterFitsTest::test_createImage()
{
    QDomElement config;
    ImageWriterFits fits(config);
    ImageData image(128,128,1,1);
    real_t* amp = image.ptr();
    for (unsigned m = 0; m < image.sizeM(); m++) {
        for (unsigned l = 0; l < image.sizeL(); l++) {
            unsigned index = m * image.sizeL() + l;
            amp[index] = static_cast<real_t>(m);
        }
    }
//    image.cellsizeL() = 1.0;
//    image.cellsizeM() = 1.0;
    fits._image = &image;
    TIMER_START
    fits.fileName() = "test";
    fits._open();
    fits._writeHeader();
    real_t* im = image.ptr();
//    fits._writeImage(im, 128, 128);
    TIMER_STOP("ImageWriterFitsTest::test_createImage()");
    fits._close();
}



} // namespace pelican
