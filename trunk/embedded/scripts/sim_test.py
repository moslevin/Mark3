### Quick script to automate the execution of all unit tests
### By running the tests on the simavr simulator
import pexpect

# Set the environment variables for the test platform
platform  = "avr"
cpu       = "atmega644"
toolchain = "gcc"
stage	= "./stage"

# List of unit tests to run
test_list = ["ut_logic", "ut_thread", "ut_semaphore", "ut_mutex", "ut_eventflag", "ut_heap", "ut_message", "ut_timers"]

# Run each test in succession
for test in test_list:
	# Build the commandline used to run the tests
	test_cmd = "simavr -f 16000000 -m %s %s/app/%s/%s/%s/%s.elf" % (cpu, stage, platform, cpu, toolchain, test)
	child = pexpect.spawn( test_cmd )
	print "--[Running Test: %s]--" % test 
	index = child.expect (["--DONE--","(FAIL)", pexpect.EOF, pexpect.TIMEOUT], timeout=240)
	if index == 0:
		print "		(PASS)"
	elif index == 1:
		print "		(FAIL)"
	elif index == 2:
		print "		(Premature EOF - did test crash?!)"
		break
	elif index == 3:
		print "		(Test timed out!)"
		break

