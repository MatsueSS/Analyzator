Tool for network interactions - Analyzer

This program will be a good tool for developing new network applications with full logging and summarizing for a selected period of server operation based on the collected logs.

----- History ------

Generating an idea, setting a task and determining the outcome - 9.01.2025

Selection of development tools, design of the structure of interaction of modules - 13.01.2025

Analysis of the selected tools, description of the entire design, writing pseudocode - 28.01.2025

Release of the first version of my program with written tests of different modules v1.0.0 - 11.02.2025

Change in user processing structure, release of new version v1.1.0 - 15.02.2025

Release of a new version with added tests and changes in program performance v1.1.2 - 17.02.2025

----- Design solutions -----

Now I want to write about my choice of these or those tools and about my analysis.
My program can be divided into 2 independent modules. One of them is the module for time control and output of the result file.
--- Module 1

I decided to make this module exactly like this. When I was previously studying Stevens' books on network interactions, it often occurred to me that monitoring and keeping track of all errors is very tedious and routine. I thought, why not design a system that will allow the developer to set the desired errors in the output, and then leave the server running. It would seem, do the usual logging and analyze your errors, but this is also routine and tedious. Why shouldn't the developer set a list of desired errors, that is, this module reads the logs in portions, and then "compresses" them and, based on the desired list of event codes and errors, produces a file about the server's operation. Perhaps many who will analyze fragments of my program will be outraged, why I used unordered_map and unordered_multimap so much, and not map/multimap or other containers. The thing is that I strictly control the size of this data structure, but in addition to this, I perform a lot of searching, insertion and deletion in it. Any other data structure will lose in the long run.
--- Module 2

I want to dedicate the next part to the 2nd module. This module is a server that works on the principle of select + pool threads. I cannot assume how many clients will interact with my server, so I set limits in the program in the form of macros. These limits are based on an error that I do not yet know how to fix.
The server is a cloud storage of encrypted user passwords. User authentication passwords are stored in the database as a hash.

----- Instructions -----

In the program, I tried to make the most friendly interface for clients, where everything seems to be described in detail
Now it's worth touching on the topic of choosing macro values. I chose such macro values ​​based on "difficult" situations in which I tested certain modules
For module 1, specifically for reading portions, I did a test where I generated about 2 million logs during the day, setting several streams with a timer for this, and then set the program to read all of this and to my surprise it was done in less than a second, and I did not see any load on the RAM.

----- Main problem -----

I want to describe the main error that I can't logically fix yet. I made such a log compression system that if a user connects, then their connection timestamp is entered into the logs. When summing up, I may be interested in the maximum communication time, so I need to track all clients. In order not to disturb the order, I decided to make it so that logs without the end of the timestamp remain in the container and wait for the timestamp. I set the size limit for this container. Let's not exclude the situation when a number of users connect that is greater than the maximum number of logs stored in the container, then all the logs will be filled and reading the logs will be disrupted. Therefore, I set a limit on clients

----- Clients Problem -----

I want to describe another problem, the solution to which I suppose, but it will greatly complicate the logic of the server. This problem is the freezing of clients. I do not process frozen clients, which is wrong. I tried to set the SO_KEEPALIVE, but tests showed that it has no effect and the client continues to hang. I see one of the solutions - creating a map of the latest activities, which will be constantly viewed by one of the threads, which is in the background in its free time, but I think that I will leave this issue, because it can clearly be implemented differently.
I recently tried to set the SO_KEEPALIVE parameter and specified the necessary other parametrs, which set the check time, frequency of sending confirmation messages and their number. I checked the server operation using tcpdump and did not notice sending packets. It seems to me that one of the solutions may be - setting global values ​​on the linux system for SO_KEEPALIVE.

----- Data base -----

The PostgreSQL was chosen as the database. In my opinion, this is the most convenient database in that each thread of your program must have a separate connection to it.

----- Performance tests -----

I placed time stamps in the client thread processing cycle. All calculations not related to accessing the database took tens of microseconds. Calculations related to accessing the database took an average of 100 milliseconds. I decided to run the client, starting with registration and ending with using all the functions. I also did it for the client with authentication. I got that on average 50 milliseconds per client. Thus, it can be said that 1 thread can process about 20 clients per second, and for 6 threads that I use in the program it comes out to 120. It is worth making an adjustment for the network and other loads, then we will get the result - my server on my device can handle approximately 100 clients.

I conducted these tests on an old laptop with 8 GB of RAM and an 8-thread processor.

Thus, the value of the macro in the new_TCP_server.h is calculated based on experiments, which I advise everyone who will use my program to do and set their own value

----- Encrypt -----

Client in example_clients uses RSA, salt 16, 100000 iteration

----- used instruments -----

C++/C, STL, git, cmake, libsodium, lidpq, openssl