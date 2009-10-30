/**
 * \brief Pipeline base class record method.
 * \details
 * Method to record data blobs (to disk). This might be called for
 * example to record images to disk at the end of the pipeline.
 */
void Pipeline::record(Converter &converter)
{ 
	// ... Work out file handle in some way
    // ... this probably has to be unique
	coverter.setData() // if needed... (depends on arguments)
	coverter.write(data, fileHandle)
    return;
}


/**
 * \brief Pipeline base class constructor.
 * \details
 * Constructs a pipeline base class. This requires:
 * - Reading configuration data and passing configuration command line
 * options(which override default options).
 * - Instantiating the data client.
 */
Pipeline::Pipeline(argc, argv)
{
	/* Retrieve options from both the command line and config file */
    readConfig(argc, argv); //parseCommandLineOptions();
	DataClient dataClient = new DataClient();
    return;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
// THE FOLLOWING IS EXAMPLE PIPELINE CODE. 
// ie. this is what to write when starting creating a new pipeline
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////


/**
 * \brief Example main for the pipeline binary for a lofar pipeline
 */
int main (argc, argv) {
	LofarPipeLine *p = new LofarPipeline(argc, argv);
	p.start();
    return 0;
}


/**
 * \brief Pipeline base class method used to put a pipeline into
 * a processing loop.
 */
void Pipeline::start()
{
	while (!end of universe) {
        getData();
		run();
	}
}


/**
 * \brief Header for an example lofar pipeline.
 * \details This class inherits from the pipeline base class and
 * impliments the virtual initialise() and run() methods from the base
 * class.
 */
Class LofarPipeline : public Pipeline
{
public:
    /// constructor
    LofarPipeline(argc, argv);
    /// Pipeline initialisation 
    void initialise();
    /// Pipeline processing loop
	void run();
};

/**
 * \brief Initialisation method for the example lofar pipeline.
 * \details Initialisation involves in Instantiation of modules 
 * and converter (modules) used for the pipeline.
 */
void LofarPipeline::initialise()
{
	/* create modules */	
	Module *flag = pipe->createModule("utility::flagging");
	Module *dft = pipe->createModule("imager::dft");
	Module *clean = pipe->createModule("imager::clean");
	Converter *fitsConverter = pipe->createConverter("converter::FITS");
    return;
}


/**
 * \brief Processing loop method for example lofar pipeline.
 * \details The processing loop specifies the data flow though the 
 * various processing modules in the pipeline as well as any 
 * output data produced by the pipeline.
 */
void LofarPipeline::run() 
{
	/* Define the pipeline and run it! */	
	flag->run(data);
	dft->run(data);
	clean->run(data);

	/* Dump some output to the data store - the 2 versions of the 
     * pipeline base class record method are shown here.
     */
	pipe->record(fitsConverter, data, "image::dft");
    fitsConverter->setData(data["image::clean"]);
	pipe->record(fitsConverter);
    return;
}

		
