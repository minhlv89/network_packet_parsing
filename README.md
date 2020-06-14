How to perform unit test

1) Extract and Build:
Extract file minhlv_packet_parser_assignment.tar.gz

cd to directory minhlv_packet_parser_assignment

$ cmake .
$ make

There should be message in terminal after make:
[100%] Built target axelspacepacketparsing

2) Check if there is executable file:
Make sure terminal is still in directory minhlv_packet_parser_assignment

$ ls -l
There should be file "axelspacepacketparsing"

3) Run:
Make sure terminal is still in directory minhlv_packet_parser_assignment

$ ./axelspacepacketparsing

Result of unit test should be:

Test case 1 is :Pass
Test case 2 is :Pass
Test case 3 is :Pass
Parser::detectPkt Bad Magic!
Test case 4 is :Pass
Parser::detectPkt Error checksum bb, 7b
Test case 5 is :Pass
Parser::extractData invalid protocol!
Test case 6 is :Pass
Parser::extractData invalid subsystemID!
Test case 7 is :Pass
Parser::extractData invalid componentID!
Test case 8 is :Pass
Parser::extractData invalid telemetry:3
Test case 9 is :Pass
