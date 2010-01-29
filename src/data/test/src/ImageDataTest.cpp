#include "ImageDataTest.h"
#include "ImageData.h"
#include "utility/memCheck.h"

namespace pelican {

CPPUNIT_TEST_SUITE_REGISTRATION( ImageDataTest );
// class DataRequirementsTest 
ImageDataTest::ImageDataTest()
    : CppUnit::TestFixture()
{
}

ImageDataTest::~ImageDataTest()
{
}

void ImageDataTest::setUp()
{
}

void ImageDataTest::tearDown()
{
}


/**
 * @details
 * Tests the various accessor methods for the image data blob
 */
void ImageDataTest::test_accessorMethods()
{
    // Use Case
    // Construct an image and test each of the accessor methods
    int nL = 1024;
    int nM = 512;
    int nChan = 2;
    ImageData image(nL, nM, nChan);
    image.setCellsizeL(1.0);
    image.setCellsizeM(2.0);
    image.setRefPixelL(512.5);
    image.setRefPixelM(256.0);
    image.setCoordType(ImageData::COORD_AZ_EL);
    image.setRefCoordL(40.0);
    image.setRefCoordM(60.25);

    for (int c = 0; c < nChan; c++) {
        for (int m = 0; m < nM; m++) {
            for (int l = 0; l < nL; l++) {
                image(l, m, c) = c + l + m;
            }
        }
    }

    CPPUNIT_ASSERT(image.nPixels() == nL * nM * nChan);
    CPPUNIT_ASSERT(image.nChannels() == nChan);
    CPPUNIT_ASSERT(image.sizeL() == nL);
    CPPUNIT_ASSERT(image.sizeM() == nM);
    CPPUNIT_ASSERT(image.cellsizeL() == 1.0);
    CPPUNIT_ASSERT(image.cellsizeM() == 2.0);
    CPPUNIT_ASSERT(image.refPixelL() == 512.5);
    CPPUNIT_ASSERT(image.refPixelM() == 256.0);
    CPPUNIT_ASSERT(image.coordType() == ImageData::COORD_AZ_EL);
    CPPUNIT_ASSERT(image.refCoordL() == 40.0);
    CPPUNIT_ASSERT(image.refCoordM() == 60.25);

    for (int c = 0; c < nChan; c++) {
        for (int m = 0; m < nM; m++) {
            for (int l = 0; l < nL; l++) {
                int index = c * nL * nM + m * nL + l;
                CPPUNIT_ASSERT(image.at(l, m, c) == c + l + m);
                CPPUNIT_ASSERT(image(l, m, c) == c + l + m);
                CPPUNIT_ASSERT(image[index] == c + l + m);
            }
        }
    }

    std::vector<real_t> im = image.amp();
    for (int c = 0; c < nChan; c++) {
        for (int m = 0; m < nM; m++) {
            for (int l = 0; l < nL; l++) {
                int index = c * nL * nM + m * nL + l;
                CPPUNIT_ASSERT(im[index] == c + l + m);
            }
        }
    }

    real_t *imPtr = image.ampPtr();
    real_t *imPtrChan0 = image.ampPtr(0);
    real_t *imPtrChan1 = image.ampPtr(1);
    for (int m = 0; m < nM; m++) {
        for (int l = 0; l < nL; l++) {
            CPPUNIT_ASSERT(imPtr[m * nL + l] == l + m);
            CPPUNIT_ASSERT(imPtr[nL * nM + m * nL + l] == 1 + l + m);
            CPPUNIT_ASSERT(imPtrChan0[m * nL + l] == l + m);
            CPPUNIT_ASSERT(imPtrChan1[m * nL + l] == 1 + l + m);
        }
    }
}


} // namespace pelican
