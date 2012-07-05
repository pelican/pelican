#ifndef ABSTRACTPIPELINE_H
#define ABSTRACTPIPELINE_H

/**
 * @file AbstractPipeline.h
 */

#include "pelican/data/DataRequirements.h"
#include "pelican/modules/AbstractModule.h"
#include "pelican/utility/FactoryConfig.h"
#include "pelican/utility/FactoryGeneric.h"

#include <QtCore/QString>
#include <QtCore/QList>

namespace pelican {

namespace test {
class TestPipeline;
}

class AbstractModule;
class DataBlob;
class PipelineDriver;
class OutputStreamManager;
class DataBlobBuffer;

/**
 * @ingroup c_core
 *
 * @class AbstractPipeline
 *
 * @brief
 * Base class for all Pelican pipeline objects.
 *
 * @details
 * This is the base class for all Pelican pipelines, which act as containers
 * for pipeline modules.
 *
 * Inherit from this class and implement the init() and run() virtual methods
 * to create a new pipeline. Set the name of the pipeline using setName(), and
 * call createModule() from init() to create the pipeline modules.
 *
 * To request remote data obtained from the client, call requestRemoteData()
 * with the type of data required. The requested data will be made available
 * via the hash supplied to run().
 *
 * For example:
 *
 * \code
 * class MyPipeline : public AbstractPipeline
 * {
 *     private:
 *         // Module pointers.
 *         ZenithImagerDft* imager;
 *         ImageWriterFits *fitsWriter;
 *
 *         // Local data blobs.
 *         ImageData* imageData;
 *
 *     public:
 *         void init()
 *         {
 *             // Create the pipeline modules.
 *             imager = (ZenithImagerDft*) createModule("ZenithImagerDft");
 *             fitsWriter = (ImageWriterFits*) createModule("ImageWriterFits");
 *
 *             // Create local data blobs.
 *             imageData = (ImageData*) createBlob("ImageData");
 *
 *             // Request remote data.
 *             requestRemoteData("AntennaPositions");
 *             requestRemoteData("VisibilityData");
 *         }
 *
 *         void run(QHash<QString, DataBlob*>& remoteData)
 *         {
 *             // Get the remote data.
 *             AntennaPositions* antPos = (AntennaPositions*)
 *                     remoteData["AntennaPositions"];
 *             VisibilityData* visData = (VisibilityData*)
 *                     remoteData["VisibilityData"];
 *
 *             // Run each module as required.
 *             imager->run(imageData, antPos, visData);
 *             fitsWriter->run(imageData);
 *         }
 * };
 * \endcode
 *
 * The run() method is called each time a new hash of data is obtained from
 * the data client, and the data hash is passed as a function argument.
 */
class AbstractPipeline
{
    public:
        /// Constructs a new abstract pipeline.
        AbstractPipeline();

        /// Destroys the abstract pipeline.
        virtual ~AbstractPipeline();

    public:

        /// Initialises the pipeline by creating the required modules
        /// (pure virtual).
        /// This method creates the required modules when the pipeline is
        /// initialised, and should be implemented in a subclass.
        /// Note that Configuration Information is available through the getConfig()
        /// method.
        virtual void init() = 0;

        /// Requests remote data from the client.
        /// This method requests a type of remote data from the client.
        void requestRemoteData(const QString& type,
                               unsigned int history = 1);

        /// History size, returns the number of DataBlobs from the
        /// specified stream to keep in memory (for access by the history()
        /// method
        unsigned int historySize(const QString& type) const;

        /// Return the remote data requirements for the pipeline.
        const DataRequirements& requiredDataRemote() const;

        /// Primary interface for pipelines to emit data from the pipeline
        void dataOutput(const DataBlob*, const QString& stream = "") const;

        /// exec the pipeline
        //  the function called by the pipeline driver
        //  will do some internal housekeeping before calling
        //  the virtual run() method
        void exec(QHash<QString, DataBlob*>& data);

        /// Defines a single iteration of the pipeline (pure virtual).
        /// This method defines what happens when the pipeline is run once,
        /// and should be implemented in a subclass. It is called each
        /// time a new hash of data is obtained from the data client. The data
        /// hash is passed as a function argument.
        ///
        /// @param[in,out] data A non-const reference to the data hash,
        ///                     which may be modified by the pipeline.
        virtual void run(QHash<QString, DataBlob*>& data) = 0;

        /// Sets the data blob factory.
        void setBlobFactory(FactoryGeneric<DataBlob>* factory);

        /// Sets the module factory.
        void setModuleFactory(FactoryConfig<AbstractModule>* factory);

        /// Sets the pipeline driver.
        void setPipelineDriver(PipelineDriver* driver);

        /// Sets the output stream manager
        void setOutputStreamManager(OutputStreamManager* osmanager);

        /// disable this pipeline from being called by the pipeline Driver
        void deactivate();

        /// return the history for the specifed data stream
        const QList<DataBlob*>& streamHistory(const QString& stream) const;

    protected:
        /// get the specified Configuration Node from the pipeline configuration
        ConfigNode config( const QString& tag, const QString& name = "" );

        /// Create a data blob using the data blob factory.
        DataBlob* createBlob(const QString& type);

        /// Create a data blob using the data blob factory in Contiguous Memory
        //  Ownership of the DataBlobBuffer is the responsibility of the caller
        template<class T>
        QList<T*> createBlobs(const QString& type, unsigned int number) {
           QList<T*> buffer;
           for( unsigned int i=0; i<number; ++i) {
               buffer.append( (T*)_blobFactory->create(type) );
           }
           return buffer;
        }

        /// Create a pipeline module using the module factory.
        AbstractModule* createModule(const QString& type,
                const QString& name = QString());


        /// Stops the pipeline driver.
        void stop();

        /// copy pipeline configuration details to the provided pipeline
        void copyConfig( AbstractPipeline* pipeline ) const;

    private:
        /// The data required by the pipeline.
        DataRequirements _requiredDataRemote;

        /// Pointer to the PipelineApplication blob factory.
        FactoryGeneric<DataBlob>* _blobFactory;

        /// Pointer to the PipelineApplication module factory.
        FactoryConfig<AbstractModule>* _moduleFactory;

        /// Pointer to the pipeline driver.
        PipelineDriver* _pipelineDriver;

        /// List of pointers to modules in this pipeline.
        QList<AbstractModule*> _modules;

        /// Pointer to the output stream manager.
        OutputStreamManager* _osmanager;

        /// Buffer Sizes required for each stream
        QHash<QString,unsigned int> _history;

        /// Historical Record of blobs passed down
        //  in reverse order (latest at the front)
        QHash<QString, QList<DataBlob*>* > _streamHistory;


    private:
        /// \todo fix me (horrible use of friend class)!
        friend class test::TestPipeline;
        friend class AbstractPipelineTest;
        friend class AbstractModule;
};

} // namespace pelican
#endif // ABSTRACTPIPELINE_H
