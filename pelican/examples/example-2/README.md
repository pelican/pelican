This benchmark tests data the throughput for a simple pelican configuration.

Results from the benchmark can then be compared to iperf to obtain an estimate
of the overhead on network throughput caused by the pelican framework.


e.g.
    iperf -u -s -l 8k  (server - listens for data)
    iperf -u -c 127.0.0.1 -l 8k -b 800M (client - sends data)
