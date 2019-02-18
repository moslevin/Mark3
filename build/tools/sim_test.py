### Quick script to automate the execution of all unit tests
### By running the tests on the flavr simulator
import pexpect

# Set the environment variables for the test platform
platform  = "avr"
cpu       = "atmega1284p"
toolchain = "gcc"
stage	= "./out/avr_atmega1284p_gcc/kernel/"
# List of unit tests to run
test_list = ["ut_logic", "ut_thread", "ut_semaphore", "ut_mutex", "ut_eventflag", "ut_message", "ut_mailbox", "ut_notify", "ut_timers", "ut_sanity", "ut_condvar", "ut_readerwriter" ]

# Run each test in succession
for test in test_list:
	# Build the commandline used to run the tests

	test_cmd = "flavr --variant %s --exitreset --silent --elffile %s/tests/unit/%s/%s.elf" % (cpu, stage, test, test)
	child = pexpect.spawn( test_cmd )

	print "--[Running Test: %s]--" % test 
	index = child.expect (["--DONE--","(FAIL)", pexpect.EOF, pexpect.TIMEOUT], timeout=240)
	if index == 0:
		print "		(PASS)"
	elif index == 1:
		print "		(FAIL)"
	elif index == 2:
		print "		(EOF!)"
		print child.before
	elif index == 3:
		print "		(FAIL -- TIMEOUT)"
		break

