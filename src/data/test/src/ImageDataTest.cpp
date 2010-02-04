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
    unsigned nL = 256;
    unsigned nM = 125;
    unsigned nChan = 2;
    unsigned nPol = 2;
    ImageData image(nL, nM, nChan, nPol);
    image.cellsizeL() = 1.0;
    image.cellsizeM() = 2.0;
    image.refPixelL() = 128.5;
    image.refPixelM() = 64.0;
    image.coordType() = ImageData::COORD_AZ_EL;
    image.refCoordL() = 40.0;
    image.refCoordM() = 60.25;

    for (unsigned p = 0; p < nPol; p++ ) {
        for (unsigned c = 0; c < nChan; c++) {
            for (unsigned m = 0; m < nM; m++) {
                for (unsigned l = 0; l < nL; l++) {
                    image(l, m, c, p) = c + l + m + p;
                }
            }
        }
    }

    CPPUNIT_ASSERT(image.nPixels() == nL * nM * nChan * nPol);
    CPPUNIT_ASSERT(image.nChannels() == nChan);
    CPPUNIT_ASSERT(image.nPolarisations() == nPol);
    CPPUNIT_ASSERT(image.sizeL() == nL);
    CPPUNIT_ASSERT(image.sizeM() == nM);

    std::vector<real_t> im = image.amp();
    real_t *imPtr = image.ampPtr();
    for (unsigned p = 0; p < nPol; p++ ) {
        for (unsigned c = 0; c < nChan; c++) {
            for (unsigned m = 0; m < nM; m++) {
                for (unsigned l = 0; l < nL; l++) {
                    unsigned index = p * nChan * nL * nM + c * nL * nM + m * nL + l;
                    real_t value = static_cast<real_t>(c + l + m + p);
                    CPPUNIT_ASSERT(image(l, m, c, p) == value);
                    CPPUNIT_ASSERT(image[index] == value);
                    CPPUNIT_ASSERT(im[index] == value);
                    CPPUNIT_ASSERT(imPtr[index] == value);
                }
            }
        }
    }

    real_t *imPtrPol0 = image.ampPtr(0);
    real_t *imPtrPol1 = image.ampPtr(1);
    for (unsigned c = 0; c < nChan; c++) {
        for (unsigned m = 0; m < nM; m++) {
            for (unsigned l = 0; l < nL; l++) {
                unsigned value = c + m + l;
                unsigned index = c * nL * nM + m * nL + l;
                CPPUNIT_ASSERT(imPtrPol0[index] == value + 0);
                CPPUNIT_ASSERT(imPtrPol1[index] == value + 1);
            }
        }
    }

    real_t *imPtrPol0Chan0 = image.ampPtr(0,0);
    real_t *imPtrPol1Chan1 = image.ampPtr(1,1);
    for (unsigned m = 0; m < nM; m++) {
        for (unsigned l = 0; l < nL; l++) {
            unsigned value = m + l;
            unsigned index = m * nL + l;
            CPPUNIT_ASSERT(imPtrPol0Chan0[index] == value + 0 + 0);
            CPPUNIT_ASSERT(imPtrPol1Chan1[index] == value + 1 + 1);
        }
    }
}


} // namespace pelican
