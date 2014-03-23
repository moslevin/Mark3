#!/bin/sh

# Do a clean build of all the code 
./build.sh

# Run the sanity test app, gather the stats
./sanity.sh

# Run the profiling test app, gather the stats (which are re-inserted into the documentation)
./profile.sh

# Building the documentation will generate the html/pdf documentation, which includes the 
# sanity/profiling data from the above tests.
./build_docs.sh


