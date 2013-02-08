+------------------------------------------------------------------------------+
| PELICAN tutorial                                Last update: 09 January 2013 |
+------------------------------------------------------------------------------+

1. Introduction
--------------------------------------------------------------------------------
This is a very simple tutorial described in the PELICAN user guide.

2. Running the example.
--------------------------------------------------------------------------------
1. Launch three separate terminals open in the tutorial binary directory.
2. Start the server binary one of the terminals by running:
    $ ./signalServer serverConfig.xml
3. Start the pipeline binary in anther of the terminals by running:
    $ ./signalPipeline pipelineConfig.xml
4. Run the emulator binary to inject some data into the server. This is done
   with by running:
    $ ./signalEmulator
    
For a description of the expected output, please refer to the PELICAN user
guide.

3. Plotting the output data.
--------------------------------------------------------------------------------
Each pipeline itteration appends a line to the csv file consisting of 
8192 floats. This can be plotted in GNUPlot with the script provided.

