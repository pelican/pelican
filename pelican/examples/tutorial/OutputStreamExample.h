#ifndef OUTPUTSTREAMEXAMPLE_H
#define OUTPUTSTREAMEXAMPLE_H
#include <QtCore/QList>
class QIODevice;

#include "pelican/output/AbstractOutputStream.h"

/**
 * @file OutputStreamExample.h
 */

using namespace pelican;

/**
 * @ingroup c_examples
 *
 * @class OutputStreamExample
 *
 * @brief
 * Writes a SignalData data blob to a CSV file.
 *
 * @details
 * Specify one or more files in the configuration XML with
 * <OutputStreamExample>
 *     <file name="file1.csv">
 *     <file name="duplicatefile.csv">
 * </OutputStreamExample>
 *
 */
class OutputStreamExample : public AbstractOutputStream
{
    public:
        // OutputStreamExample constructor.
        OutputStreamExample(const ConfigNode& configNode);

        // OutputStreamExample destructor.
        ~OutputStreamExample();

        // Add a file for output to be saved.
        void addFile(const QString& filename);

    protected:
        // Sends the data blob to the output stream.
        void sendStream(const QString& streamName, const DataBlob* dataBlob);

    private:
        QList<QIODevice*> _devices;
};

PELICAN_DECLARE(AbstractOutputStream, OutputStreamExample)

#endif // OUTPUTSTREAMEXAMPLE_H
