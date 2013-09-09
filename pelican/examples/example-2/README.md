+------------------------------------------------------------------------------+
| PELICAN Example-2                             Last update: 05 September 2013 |
+------------------------------------------------------------------------------+

1. Introduction
--------------------------------------------------------------------------------
This example demonstrates measuring throughput performance of a TCP data stream
as well as logging the state of the server buffers from the Chunker. 


2. Running the example.
--------------------------------------------------------------------------------
1. Launch three separate terminals open in the example-2 binary directory.
2. Start the server binary one of the terminals by running:
    $ ./server server.xml
3. Start the pipeline binary in anther of the terminals by running:
    $ ./pipeline pipeline.xml
4. Run the emulator binary to inject some data into the server. This is done
   with by running:
    $ ./emulator emulator.xml [configuration]
   where configuration is one of the emulator XML node names, currently 1,2,3 
   or 4.
   

4. Comparing pelican throughput to iperf
--------------------------------------------------------------------------------
Results from the benchmark can then be compared to iperf to obtain an estimate
of the overhead on network throughput caused by the pelican framework.

e.g.
    iperf -u -s -l 8k  (server - listens for data)
    iperf -u -c 127.0.0.1 -l 8k -b 800M (client - sends data)
