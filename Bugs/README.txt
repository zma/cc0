This directory is used for bug reporting and tracking

Open: This directory contains the test cases that reproduce the newly reported bugs.
Fixed: Test cases of fixed (but not verified bugs.
Closed: Test cases of fixed bugs. (All test cases in this folder should pass.)
Invalid: Test cases for invalid (false) bugs.

A description and changelog should be maintained in the comment at the begining of the test case.

The procedure of reporting and fixing a bug.

1. User A writes a SIMPLE test case that reproduce the bug, and put it into the Open directory. Simple means minimal. In other words, the smaller the better.
2. a) If someone, say Developer B, fixed this bug, and put the test case into the Fixed directory.
   b) User A or anyone other than B, verify that the bug is really fixed, and put the test case into Closed directory. Note that the one who fixed the bug should not verify the fix.
3. a) If someone, say developer C, find that it is a false bug report, it put the test case into the Invalid directory.
   b) User A or anyone other than C, verify that it is indeed a false report. Then he delete the test case. 

