#include "pelican/core/DirectStreamDataClient.h"
#include "pelican/utility/Config.h"
#include "pelican/utility/FactoryConfig.h"
#include "pelican/utility/FactoryGeneric.h"
#include "pelican/core/DataClientFactory.h"
#include "pelican/core/AbstractAdapter.h"
#include "pelican/data/DataBlob.h"
#include "pelican/data/ArrayData.h"
#include "pelican/server/test/TestUdpChunker.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QTime>
#include <QtCore/QString>
#include <QtCore/QList>
#include <QtCore/QHash>
#include <iostream>

using namespace pelican;

//static void printData(DoubleData* data)
//{
//    for (unsigned i = 0; i < data->size(); ++i) {
//        std::cout << "Data " << data->ptr()[i] << std::endl;
//    }
//}

int main(int argc, char** argv)
{
    try {
    QCoreApplication app(argc, argv);

    // Get command line arguments.
    if (argc != 5) {
        std::cerr << "Usage: directClient <port> <host> <chunk size, bytes> <iterations>" << std::endl;
        return 1;
    }

    int port = atoi(argv[1]);
    QString host(argv[2]);
    int chunkSize = atoi(argv[3]);
    int iterations = atoi(argv[4]);

    QString pipelineXml = ""
            "<buffers>"
            "   <DoubleData>"
            "       <buffer maxSize=\"2000000\" maxChunkSize=\"2000000\"/>"
            "   </DoubleData>"
            "</buffers>"
            "<chunkers>"
            "    <TestUdpChunker name=\"a\">"
            "       <connection host=\"" + host + "\" port=\"" + QString::number(port) + "\"/>"
            "       <data type=\"DoubleData\" chunkSize=\"" + QString::number(chunkSize) + "\"/>"
            "    </TestUdpChunker>"
            "</chunkers>"
            "<clients>"
            "    <DirectStreamDataClient>"
            "        <data type=\"DoubleData\" adapter=\"AdapterRealData\"/>"
            "    </DirectStreamDataClient>"
            "</clients>"
            "<adapters>"
            "</adapters>";
    Config config;
    config.setFromString(pipelineXml);

    // Create the adapter factory.
    AbstractAdapterFactory adapterFactory(&config, "pipeline", "adapters");

    // Create the data client factory.
    DataClientFactory clientFactory(&config, "pipeline", "clients", &adapterFactory);

    // Create the data blob factory.
    //FactoryGeneric<DataBlob> blobFactory;

    // Create a list of data requirements.
    QString dataType = "DoubleData";
    DataRequirements req;
    QList<DataRequirements> requirements;
    req.addStreamData(dataType);
    requirements.append(req);

    // Create the client.
    DirectStreamDataClient* client = static_cast<DirectStreamDataClient*>(
            clientFactory.create("DirectStreamDataClient", requirements));
    client->addStreamChunker("TestUdpChunker", "a");

    // Set up the data hash.
    //QHash<QString, DataBlob*> dataHash;
    //dataHash.insert(dataType, blobFactory.create(dataType));

    // Initialise loop counter.
    QTime timer;
    timer.start();
    int counter = 0;
    double initValue = 0, value = 0;
    for (counter = 0; counter < iterations; counter++) {
        // Get the data.
//        QHash<QString, DataBlob*> validData = client->getData(dataHash);

        // Check the content of the data blob.
//        DoubleData* realData = (DoubleData*)validData.value(dataType);
//        //printData(realData);
//        value = realData->ptr()[0];
//        if (counter == 0)
//            initValue = value;
    }

    // Check for lost packets.
    double dataRange = value - initValue;
    double lostPackets = dataRange + 1 - iterations;

    // Compute bandwidth.
    double sec = (double)timer.elapsed() / 1e3;
    long double megaBytesReceived = (long double)chunkSize * iterations / (1024 * 1024);

    // Print summary.
    std::cout << "---------------------------------------------------------\n";
    std::cout << "Data range " << dataRange << " over " << iterations
            << " iterations. (Missed " << lostPackets << " packets.)\n";
    std::cout << "Received " << megaBytesReceived << " MiB in "
            << sec << " seconds.\n";
    std::cout << "Bandwidth: " << ((megaBytesReceived * 8) / sec)
            << " Mb/sec" << std::endl;
    std::cout << "---------------------------------------------------------\n";

    } catch (QString error) {
        std::cerr << error.toStdString() << std::endl;
    }

    return 0;
}

