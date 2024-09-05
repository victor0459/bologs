# Project Introduction
    This is a C, C++-based load balancing framework
    The project consists of four modules, lars_reactor、lars_dns service、lars_report service、lars_load balance agent Load Agent
    ⭐lars_reactor：A Linux C++ server framework model based on the Reactor patternIncluding Turk Communication, Ode Communication
    ⭐lars_dns service：Responsible for receiving messages and returning available nodes (explanation: providing routing services for agents)
    ⭐lars_report service：It is responsible for receiving the report of the call status of each agent node.
    ⭐lars_load balance agent：It has 5 threads and one LB algorithm
    ⭐Various language interfaces are included, such as gram, gram++, and Jawa Paisen
    ⭐IO multiplexing is used in the project, and a large number of callbacks, queues, etc. are used to improve concurrency
    ⭐Application scenarios：Large-scale portals and e-commerce rush purchases...
    ⭐advantage：The cluster supports tens of millions of concurrent connections to meet the massive business access needs of users. Automatic distribution, seamless integration with auto scaling, and flexible expansion of external service capabilities used by users




# Description of the directory structure
    ├── lars
    │   ├── lars_dns
    │   │   ├── conf
    │   │   │   └── lars_dns.ini
    │   │   ├── include
    │   │   │   └── subcribe.h
    │   │   ├── src
    │   │   │   └── subcribe.cpp
    │   │   └── test
    │   ├── lars_lb_agent
    │   ├── lars_reactors
    │   │   ├── example
    │   │   │   ├── client
    │   │   │   ├── client.cpp
    │   │   │   ├── Makefile
    │   │   │   ├── qps_test
    │   │   │   │   ├── build.sh
    │   │   │   │   ├── client
    │   │   │   │   ├── client.cpp
    │   │   │   │   ├── echoMessage.pb.cc
    │   │   │   │   ├── echoMessage.pb.h
    │   │   │   │   ├── echoMessage.proto
    │   │   │   │   ├── Makefile
    │   │   │   │   ├── reactor.ini
    │   │   │   │   ├── server
    │   │   │   │   └── server.cpp
    │   │   │   ├── reactor.ini
    │   │   │   ├── server
    │   │   │   └── server.cpp
    │   │   ├── include
    │   │   │   ├── buf_pool.h
    │   │   │   ├── config_file.h
    │   │   │   ├── event_base.h
    │   │   │   ├── event_loop.h
    │   │   │   ├── io_buf.h
    │   │   │   ├── lars_reactor.h
    │   │   │   ├── message.h
    │   │   │   ├── net_connection.h
    │   │   │   ├── reactor_buf.h
    │   │   │   ├── task_msg.h
    │   │   │   ├── tcp_client.h
    │   │   │   ├── tcp_conn.h
    │   │   │   ├── tcp_server.h
    │   │   │   ├── thread_pool.h
    │   │   │   ├── thread_queue.h
    │   │   │   ├── udp_client.h
    │   │   │   └── udp_server.h
    │   │   ├── lib
    │   │   │   └── libreactor.a
    │   │   ├── Makefile
    │   │   └── src
    │   │       ├── buf_pool.cpp
    │   │       ├── buf_pool.o
    │   │       ├── config_file.cpp
    │   │       ├── config_file.o
    │   │       ├── event_loop2.cpp
    │   │       ├── event_loop.cpp
    │   │       ├── event_loop.o
    │   │       ├── io_buf.cpp
    │   │       ├── io_buf.o
    │   │       ├── reactor_buf.cpp
    │   │       ├── reactor_buf.o
    │   │       ├── tcp_client.cpp
    │   │       ├── tcp_client.o
    │   │       ├── tcp_conn.cpp
    │   │       ├── tcp_conn.o
    │   │       ├── tcp_server.cpp
    │   │       ├── tcp_server.o
    │   │       ├── thread_pool.cpp
    │   │       ├── thread_pool.o
    │   │       ├── udp_client.cpp
    │   │       ├── udp_client.o
    │   │       ├── udp_server.cpp
    │   │       └── udp_server.o
    │   ├── lars_reporters
    │   │   ├── conf
    │   │   │   └── lars_reporter.ini
    │   │   ├── include
    │   │   │   └── store_report.h
    │   │   ├── src
    │   │   │   ├── reporter_service2.cpp
    │   │   │   ├── reporter_service.cpp
    │   │   │   ├── store_report.cpp
    │   │   │   └── store_thread.cpp
    │   │   └── test
    │   │       ├── Makefile
    │   │       ├── reportClient.cpp
    │   │       ├── sep_zefo.cpp
    │   │       ├── sep_zeon.cpp
    │   │       ├── sep_zeth.cpp
    │   │       └── sep_zetw.cpp
    │   └── lars_web
    ├── LICENSE
    └── README.md



 

# Version content updates
###### v1.0: 
    1.The lars_reactors build is complete
    
    2.Completed the tests of the lars_reactors
    
    3.Completed the testing of the QPS of the lars_reactors framework

 v2.0: 

    1.The lars_reporters build is complete
    

# Project schedule and schedule



    Starting on 2024-09-01, the test of the lars_reporters module will be completed, and then the construction of the lars_dns module will be completed
    
    
    

 
