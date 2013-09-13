+------------------------------------------------------------------------------+
| PELICAN Example-1                               Last update: 09 January 2013 |
+------------------------------------------------------------------------------+

1. Introduction
--------------------------------------------------------------------------------
This is a very simple example of using pelican with service and stream data.

2. Description
--------------------------------------------------------------------------------
PELICAN can make use of two 'types' of data, distinguished by the way they are
interact with the running of pipelines. The first type of data called 'stream data'
is required for each iteration of a pipeline and is therefore fundamental to
writing applications using the PELICAN framework. A second data type, called 
'service data' is also support. This is a data type that can be used when writing
pipelines for data that updates less frequency than a single pipeline iteration.
This example demonstrates how to write a pipeline which makes use of the service
data type.


2. Running the example.
--------------------------------------------------------------------------------
1. Launch three separate terminals open in the example-1 binary directory.
2. Start the server binary one of the terminals by running:
    $ ./server server.xml
3. Start the pipeline binary in anther of the terminals by running:
    $ ./pipeline pipeline.xml
4. Run the emulator binary to inject some data into the server. This is done
   with by running:
    $ ./emulator

Once complete, the emulator can be run repeatedly to generate more data. If
working successfully, the emulator should report on the terminal that it is
sending stream and service data and the pipeline will report the data arrival.

3. Exiting from the example.
--------------------------------------------------------------------------------
The emulator binary will exit once it has finished sending its data.
The server binary should be terminated by issuing a CTRL-c command in its 
terminal and the pipeline binary should be terminated by issuing two
CTRL-c commands in its terminal 

(Note: Usually two kill signals are required to stop the pipeline binary as the 
pipeline supports a mode where the first signal will instruct it to exit at the
next chunk in the stream. In this example, it is likely that the pipeline is 
terminated after the stream has halted, with no more data arriving from the
server, therefore a second CTRL-c signal is required to instruct the pipeline 
to terminate irrespective of its current state of processing.) 



 