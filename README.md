# ros_topic_vs_actionlib
Benchmark ROS Topics and Action client/server communication.

Building:
=========
catkin build ros_actionlib_benchmark -DCMAKE_BUILD_TYPE=RELEASE

actionlib_benchmark: Simple actionlib benchmarks
================================================

This package provides implementations of action servers and clients in C++,
Python and Lua which call actions to each other and gives an impression of
round trip times and where the time is actually spent. It uses NTP-like
time stamps for measuring transmission and processing time.

The programs are very simple. But they helped us to get an idea of where we
needed to improve for better results, i.e. faster execution of actions.

Requirements
------------
The following software packages are needed on your system in order to
use actionlib_lua. Requirements marked with (*) have specific information
further down below.

- Lua 5.1			http://www.lua.org
- roslua 0.4                    http://github.com/timn/roslua
- actionlib (C-Turtle)		http://ros.org/wiki/actionlib

Usage
-----
Just run any combination of one server and one action client at a time. The
clients take a number argument which is the number of runs to do.

Some results
------------
# ./cpp_roundtrip_client 100
SEND   Average: 0.000400    Deviation: 0.000035
RECV   Average: 0.000311    Deviation: 0.000094
TOTAL  Average: 0.000711    Deviation: 0.000099

# ./python_roundtrip_client 100
SEND   Average: 0.000589    Deviation: 0.000043
RECV   Average: 0.002129    Deviation: 0.000661
TOTAL  Average: 0.002718    Deviation: 0.000661

# ./lua_roundtrip_client 100
SEND   Average: 0.000447    Deviation: 0.000116
RECV   Average: 0.002326    Deviation: 0.001108
TOTAL  Average: 0.002774    Deviation: 0.001152


# ./cpp_roundtrip_client 1000
SEND   Average: 0.000393    Deviation: 0.000036
RECV   Average: 0.000269    Deviation: 0.000025
TOTAL  Average: 0.000662    Deviation: 0.000054

# ./python_roundtrip_client 1000
SEND   Average: 0.000653    Deviation: 0.000041
RECV   Average: 0.002178    Deviation: 0.000927
TOTAL  Average: 0.002831    Deviation: 0.000934

# ./lua_roundtrip_client 1000
SEND   Average: 0.000517    Deviation: 0.000090
RECV   Average: 0.003955    Deviation: 0.001797
TOTAL  Average: 0.004471    Deviation: 0.001814

The times on higher number of runs are directly influenced by how long it takes
to deserialize the growing status message. So to get an impression how actionlib
can perform for your application you should have the number to be about the number
of goals you expect to be active in about 5 seconds of average system run-time.
See also the roslua package README for some more detailed information of the
results from above.
