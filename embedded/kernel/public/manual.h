/*===========================================================================
	 _____		  _____		   _____		_____
 ___|	 _|__  __|_	   |__	__|__	|__	 __| __	 |__  ______
|	 \	/  | ||	   \	  ||	 |	   ||  |/ /		||___	|
|	  \/   | ||		\	  ||	 \	   ||	  \		||___	|
|__/\__/|__|_||__|\__\	__||__|\__\	 __||__|\__\  __||______|
	|_____|		 |_____|	  |_____|	   |_____|

--[Mark3 Realtime Platform]--------------------------------------------------

Copyright (c) 2012 Funkenstein Software Consulting, all rights reserved.
See license.txt for more information
===========================================================================*/
/*!
	\file manual.h
	
	\brief Ascii-format documentation, used by doxygen to create various
		   printable and viewable forms.
*/
/*!
	\mainpage The Mark3 Realtime Kernel

	\verbatim	 
		 _____		  _____		   _____		_____
	 ___|	 _|__  __|_	   |__	__|__	|__	 __| __	 |__  ______
	|	 \	/  | ||	   \	  ||	 |	   ||  |/ /		||___	|
	|	  \/   | ||		\	  ||	 \	   ||	  \		||___	|
	|__/ __/|__|_||__| __\	__||__| __\	 __||__| __\  __||______|
		|_____|		 |_____|	  |_____|	   |_____|

	--[Mark3 Realtime Platform]--------------------------------------------------

	Copyright (c) 2012-2013 Funkenstein Software Consulting, all rights reserved.
	See license.txt for more information
		
	\endverbatim

	The Mark3 Realtime Kernel is a completely free, open-source, real-time 
	operating system aimed at bringing multitasking to microcontroller 
	systems without MMUs. 

	It uses modern programming languages and concepts (it's written 
	entirely in C++) to minimize code duplication, and its object-oriented 
	design enhances readibility. The API is simple - there are only six 
	functions required to set up the kernel, initialize threads, and start 
	the scheduler. 
		
	The source is fully-documented with example code provided to illustrate 
	concepts. The result is a performant RTOS, which is easy to read, easy 
	to understand, and easy to extend to fit your needs. 

	But Mark3 is bigger than just a real-time kernel, it also contains a 
	number of class-leading features: 
	
	- Device driver HAL which provides a meaningful abstraction around device-specific peripherals.	 
	- Capable recursive-make driven build system which can be used to build all libraries, examples, tests, and documentation for any number of targets from the command-line.
	- Graphics and UI code designed to simplify the implementation of systems using displays, keypads, joysticks, and touchscreens
	- Standards-based custom communications protocol used to simplify the creation of host tools
	- A bulletproof, well-documented bootloader for AVR microcontrollers
	.	 
*/
/*!		   
	\page PREFACE Preface

	\section PRE1 Who should read this

	As the cover clearly states, this is a book about the Mark3 real-time 
	kernel.	 I assume that if you're reading this book you have an interest 
	in some, if not all, of the following subjects:
	- Embedded systems
	- Real-time systems
	- Operating system kernel design
	.	
	And if you're interested in those topics, you're likely familiar with C 
	and C++ and the more you know, the easier you'll find this book to read. 
	And if C++ scares you, and you don't like embedded, real-time systems, 
	you're probably looking for another book. If you're unfamiliar with RTOS 
	fundamentals, I highly suggest searching through the vast amount of 
	RTOS-related articles on the internet to familiarize yourself with the 
	concepts. 

	\section PRE2 Why Mark3? 

	My first job after graduating from university in 2005 was with a small 
	company that had a very old-school, low-budget philosophy when it came 
	to software development. Every make-or-buy decision ended with "make" 
	when it came to tools. It was the kind of environment where vendors cost 
	us money, but manpower was free. In retrospect, we didn't have a ton of 
	business during the time that I worked there, and that may have had 
	something to do with the fact that we were constantly short on ready
	cash for things we could code ourselves. 

	Early on, I asked why we didn't use industry-standard tools - like JTAG 
	debuggers or IDEs. One senior engineer scoffed that debuggers were tools 
	for wimps - and something that a good programmer should be able to do 
	without. After all - we had serial ports, GPIOs, and a bi-color LED on 
	our boards. Since these were built into the hardware, they didn't cost 
	us a thing. We also had a single software "build" server that took 5 
	minutes to build a 32k binary on its best days, so when we had to debug 
	code, it was a painful process of trial and error, with lots of Youtube 
	between iterations. We complained that tens of thousands of dollars of 
	productivity was being flushed away that could have been solved by 
	implementing a proper build server - and while we eventually got our 
	wish, it took far more time than it should have. 

	Needless to say, software development was painful at that company. We 
	made life hard on ourselves purely out of pride, and for the right to 
	say that we walked "up-hills both ways through 3 feet of snow, 
	everyday". Our code was tied ever-so-tightly to our hardware platform, 
	and the system code was indistinguishable from the application. While we 
	didn't use an RTOS, we had effectively implemented a 3-priority 
	threading scheme using a carefully designed interrupt nesting scheme 
	with event flags and a while(1) superloop running as a background 
	thread. Nothing was abstracted, and the code was always optimized for 
	the platform, presumably in an effort to save on code size and wasted 
	cycles. I asked why we didn't use an RTOS in any of our systems and 
	received dismissive scoffs - the overhead from thread switching and 
	maintaining multiple threads could not be tolerated in our systems 
	according to our chief engineers. In retrospect, our ad-hoc system was 
	likely as large as my smallest kernel, and had just as much context 
	switching (althrough it was hidden by the compiler). 

	And every time a new iteration of our product was developed, the 
	firmware took far too long to bring up, because the algorithms and data
	structures had to be re-tooled to work with the peripherals and sensors 
	attached to the new boards. We worked very hard in an attempt to 
	reinvent the wheel, all in the name of producing "efficient" code. 

	Regardless, I learned a lot about software development. 

	Most important, I learned that good design is the key to good software; 
	and good design doesn't have to come at a price. In all but the smallest 
	of projects, the well-designed, well-abstracted code is not only more 
	portable, but it's usually smaller, easier to read, and easier to reuse. 

	Also, since we had all the time in the world to invest in developing our 
	own tools, I gained a lot of experience building them, and making use of 
	good, free PC tools that could be used to develop and debug a large 
	portion of our code. I ended up writing PC-based device and peripheral 
	simulators, state-machine frameworks, and abstractions for our horrible 
	ad-hoc system code. At the end of the day, I had developed enough tools 
	that I could solve a lot of our development problems without having to 
	re-inventing the wheel at each turn. Gaining a background in how these 
	tools worked gave me a better understanding of how to use them - making 
	me more productive at the jobs that I've had since. 

	I am convinced that designing good software takes honest effort 
	up-front, and that good application code cannot be written unless it is 
	based on a solid framework. Just as the wise man builds his house on 
	rocks, and not on sand, wise developers write applications based on a 
	well-defined platforms. And while you can probably build a house using 
	nothing but a hammer and sheer will, you can certainly build one a lot 
	faster with all the right tools. 

	This conviction lead me to development my first RTOS kernel in 2009 - 
	FunkOS. It is a small, yet surprisingly full-featured kernel. It has all 
	the basics (semaphores, mutexes, round-robin and preemptive scheduling), 
	and some pretty advanced features as well (device drivers and other 
	middleware). However, it had two major problems - it doesn't scale well, 
	and it doesn't support many devices.

	While I had modest success with this kernel (it has been featured on 
	some blogs, and still gets around 125 downloads a month), it was nothing 
	like the success of other RTOS kernels like uC/OS-II and FreeRTOS. To be 
	honest, as a one-man show, I just don't have the resources to support 
	all of the devices, toolchains, and evaluation boards that a real vendor 
	can. I had never expected my kernel to compete with the likes of them, 
	and I don't expect Mark3 to change the embedded landscape either. 

	My main goal with Mark3 was to solve the technical shortfalls in the 
	FunkOS kernel by applying my experience in kernel development. As a
	result, Mark3 is better than FunkOS in almost every way; it scales better,
	has lower interrupt latency, and is generally more thoughtfully designed 
	(all at a small cost to code size). 
	
	Another goal I had was to create something easy to understand, that 
	could be documented and serve as a good introduction to RTOS kernel 
	design. The end result of these goals is the kernel as presented in this 
	book - a full source listing of a working OS kernel, with each module 
	completely documented and explained in detail. 
	
	Finally, I wanted to prove that a kernel written entirely in C++ could
	perform just as well as one written in C, without incurring any extra
	overhead.  Comparing the same configuration of Mark2 to Mark3, the 
	code size is remarkably similar, and the execution performance is 
	just as good.  Not only that, but there are fewer lines of code.  The
	code is more readable and easier to understand as a result of making use
	of object-oriented concepts provided by C++.  Applications are easier 
	to write because common concepts are encapsulated into objects (Threads,
	Semaphores, Mutexes, etc.) with their own methods and data, as opposed
	to APIs which rely on lots of explicit pointer-passing, type casting,
	and other operations that are typically considered "unsafe" or
	"advaned topics" in C.	
*/
/*!
	\page COST Can you Afford an RTOS? 

	Of course, since you're reading the manual for an RTOS that I've been
	developing for the last few years, you can guess that the conclusion
	that I draw is a resounding "yes".
	
	If your code is of any sort of non-trivial complexity (say, at least
	a few-thousand lines), then a more appropriate question would be 
	"can you afford *not* to use an	   RTOS in your system?".
	
	In short, there are simply too many benefits of an RTOS to ignore.
	- Sophisticated synchronization objects 
	- The ability to efficiently block and wait 
	- Enhanced responsiveness for high-priority tasks
	- Built in timers
	- Built in efficient memory management
	.
	
	Sure, these features have a cost in code space and RAM, but from my 
	experience the cost of trying to code around a lack of these features 
	will cost you as much - if not more.  The results are often far
	less maintainable, error prone, and complex.  And that simply adds
	time and cost.	Real developers ship, and the RTOS is quickly becoming
	one of the standard tools that help keep developers shipping.
	
	\section COST0 Intro
	(Note - this article was written for the C-based Mark2 kernel, which
	is slightly different.	While the general principles are the same,
	the numbers are not an 100% accurate reflection of the current costs of 
	the Mark3 kernel.)
	
	One of the main arguments against using an RTOS in an embedded project 
	is that the overhead incurred is too great to be justified. Concerns 
	over "wasted" RAM caused by using multiple stacks, added CPU 
	utilization, and the "large" code footprint from the kernel cause a 
	large number of developers to shun using a preemptive RTOS, instead 
	favoring a non-preemptive, application-specific solution. 

	I believe that not only is the impact negligible in most cases, but that 
	the benefits of writing an application with an RTOS can lead to savings 
	around the board (code size, quality, reliability, and development 
	time). While these other benefits provide the most compelling case for 
	using an RTOS, they are far more challenging to demonstrate in a 
	quantitative way, and are clearly documented in numerous industry-based 
	case studies. 

	While there is some overhead associated with an RTOS, the typical 
	arguments are largely unfounded when an RTOS is correctly implemented in 
	a system. By measuring the true overhead of a preemptive RTOS in a 
	typical application, we will demonstrate that the impact to code space, 
	RAM, and CPU usage is minimal, and indeed acceptable for a wide range of 
	CPU targets. 

	To illustrate just how little an RTOS impacts the size of an embedded 
	software design we will look at a typical microcontroller project and 
	analyze the various types of overhead associated with using a 
	pre-emptive realtime kernel versus a similar non-preemptive event-based 
	framework. 

	RTOS overhead can be broken into three distinct areas: 

	- Code space: The amount of code space eaten up by the kernel (static) 
	- Memory overhead: The RAM associated wtih running the kernel and application threads. 
	- Runtime overhead: The CPU cycles required for the kernel's functionality (primarily scheduling and thread switching)
	.
	
	While there are other notable reasons to include or avoid the use of an 
	RTOS in certain applications (determinism, responsiveness, and interrupt 
	latency among others), these are not considered in this discussion - as 
	they are difficult to consider for the scope of our "canned" 
	application. Application description: 

	For the purpose of this comparison, we first create an application using 
	the standard preemptive Mark3 kernel with 2 system threads running: A 
	foreground thread and a background thread. This gives three total 
	priority levels in the system - the interrupt level (high), and two 
	application priority threads (medium and low), which is quite a common 
	paradigm for microcontroller firmware designs. The foreground thread 
	processes a variety of time-critical events at a fixed frequency, while 
	the background thread processes lower priority, aperiodic events. When 
	there are no background thread events to process, the processor enters 
	its low-power mode until the next interrupt is acknowledged. 

	The contents of the threads themselves are unimportant for this 
	comparison, but we can assume they perform a variety of I/O using 
	various user-input devices and a serial graphics display. As a result, a 
	number of Mark3 device drivers are also implemented. 

	The application is compiled for an ATMega328p processor which contains 
	32kB of code space in flash, and 2kB of RAM, which is a lower-mid-range 
	microcontroller in Atmel's 8-bit AVR line of microcontrollers. Using the 
	WinAVR GCC compiler with -O2 level optimizations, an executable is 
	produced with the following code/RAM utilization: 

	31600 Bytes Code Space 2014 Bytes RAM 

	An alternate version of this project is created using a custom 
	"super-loop" kernel, which uses a single application thread and provides 
	2 levels of priority (interrupt and application). In this case, the 
	event handler processes the different priority application events to 
	completion from highest to lowest priority. 

	This approach leaves the application itself largely unchanged. Using the 
	same optimization levels as the preemptive kernel, the code compiles as 
	follows: 

	29904 Bytes Code Space 1648 Bytes RAM 

	\section COST1 Memory overhead: 

	At first glance, the difference in RAM utilization seems quite a lot 
	higher for the preemptive mode version of the application, but the raw 
	numbers don't tell the whole story. 

	The first issue is that the cooperative-mode total does not take into 
	account the system stack - whereas these values are included in the 
	totals for RTOS version of the project. As a result, some further 
	analysis is required to determine how the stack sizes truly compare. 

	In cooperative mode, there is only one thread of execution - so 
	considering that multiple event handlers are executed in turn, the stack 
	requirements for cooperative mode is simply determined by those of the 
	most stack-intensive event handler. 

	In contrast, the preemptive kernel requires a separate stack for each 
	active thread, and as a result the stack usage of the system is the sum 
	of the stacks for all threads. 

	Since the application and idle events are the same for both preemptive 
	and cooperative mode, we know that their (independent) stack 
	requirements will be the same in both cases. 

	For cooperative mode, we see that the idle thread stack utilization is 
	lower than that of the application thread, and so the application 
	thread's determines the stack size requirement. Again, with the 
	preemptive kernel the stack utilization is the sum of the stacks defined 
	for both threads. 

	As a result, the difference in overhead between the two cases becomes 
	the extra stack required for the idle thread - which in our case is (a 
	somewhat generous) 64 bytes. 

	The numbers still don't add up completely, but looking into the linker 
	output we see that the rest of the difference comes from the extra data 
	structures used to declare the threads in preemptive mode. 

	With this taken into account, the true memory cost of a 2-thread system 
	ends up being around 150 bytes of RAM - which is less than 8% of the 
	total memory available on this particular microcontroller. Whether or 
	not this is reasonable certainly depends on the application, but more 
	importantly, it is not so unreasonable as to eliminate an RTOS-based 
	solution from being considered. 

	\section COST2 Code Space Overhead: 

	The difference in code space overhead between the preemptive and 
	cooperative mode solutions is less of an issue. Part of this reason is 
	that both the preemptive and cooperative kernels are relatively small, 
	and even an average target device (like the Atmega328 we've chosen) has 
	plenty of room. 

	Mark3 can be configured so that only features necessary for the 
	application are included in the RTOS - you only pay for the parts of the 
	system that you use. In this way, we can measure the overhead on a 
	feature-by-feature basis, which is shown below for the kernel as 
	configured for this application: 
		
	3466  Bytes

	The configuration tested in this comparison uses the thread/port module 
	with timers, drivers, and semaphores, for a total kernel size of ~3.5KB, 
	with the rest of the code space occupied by the application. 

	The custom cooperative-mode framework has a similar structure which is 
	broken down by module as follows: 

	1850  Bytes 

	As can be seen from the compiler's output, the difference in code space 
	between the two versions of the application is about 1.7kB - or about 5% 
	of the available code space on the selected processor. While nearly all 
	of this comes from the added overhead of the kernel, the rest of the 
	difference comes the changes to the application necessary to facilitate 
	the different frameworks. 

	\section COST3 Runtime Overhead

	On the cooperative kernel, the overhead associated with running the 
	thread is the time it takes the kernel to notice a pending event flag 
	and launch the appropriate event handler, plus the timer interrupt 
	execution time. 

	Similarly, on the preemptive kernel, the overhead is the time it takes 
	to switch contexts to the application thread, plus the timer interrupt 
	execution time. 

	The timer interrupt overhead is similar for both cases, so the overhead 
	then becomes the difference between the following: 

	Preemptive mode:
	- Posting the semaphore that wakes the high-priority thread 
	- Performing a context switch to the high-priority thread 
	.
	
	Cooperative mode:
	- Setting the high-priority thread's event flag 
	- Acknowledging the event from the event loop 
	.
	
	Using the cycle-accurate AVR simulator, we find the end-to-end event 
	sequence time to be 20.4us for the cooperative mode scheduler and 44.2us 
	for the preemptive, giving a difference of 23.8us. 

	With a fixed high-priority event frequency of 33Hz, we achieve a runtime 
	overhead of 983.4us per second, or 0.0983% of the total available CPU 
	time. Now, obviously this value would expand at higher event frequencies 
	and/or slower CPU frequencies, but for this typical application we find 
	the difference in runtime overhead to be neglible for a preemptive 
	system. Analysis: 

	For the selected test application and platform, including a preemptive 
	RTOS is entirely reasonable, as the costs are low relative to a 
	non-preemptive kernel solution. But these costs scale relative to the 
	speed, memory and code space of the target processor. Because of these 
	variables, there is no "magic bullet" environment suitable for every 
	application, but Mark3 attempts to provide a framework suitable for a 
	wide range of targets. 

	On the one hand, if these tests had been performed on a higher-end 
	microcontroller such as the ATMega1284p (containing 128kB of code space 
	and 16kB of RAM), the overhead would be in the noise. For this type of 
	resource-rich microcontroller, there would be no reason to avoid using 
	the Mark3 preemptive kernel. 

	Conversely, using a lower-end microcontroller like an ATMega88pa (which 
	has only 8kB of code space and 1kB of RAM), the added overhead would 
	likely be prohibitive for including a preemptive kernel. In this case, 
	the cooperative-mode kernel would be a better choice. 

	As a rule of thumb, if one budgets 10% of a microcontroller's code 
	space/RAM for a preemptive kernel's overhead, you should only require at 
	minimum a microcontroller with 16k of code space and 2kB of RAM as a 
	base platform for an RTOS. Unless there are serious constraints on the 
	system that require much better latency or responsiveness than can be 
	achieved with RTOS overhead, almost any modern platform is sufficient 
	for hosting a kernel. In the event you find yourself with a 
	microprocessor with external memory, there should be no reason to avoid 
	using an RTOS at all. 
*/
/*!
	\page SUPER Superloops	
	
	\section SUPER0 Intro to Superloops
	
	Before we start taking a look at designing a real-time operating system, 
	it's worthwhile taking a look through one of the most-common design 
	patterns that developers use to manage task execution in embedded 
	systems - Superloops. 

	Systems based on superloops favor the system control logic baked 
	directly into the application code, usually under the guise of 
	simplicity, or memory (code and RAM) efficiency. For simple systems, 
	superloops can definitely get the job done. However, they have some 
	serious limitations, and are not suitable for every kind of project. In 
	a lot of cases you can squeak by using superloops - especially in 
	extremely constrained systems, but in general they are not a solid basis 
	for reusable, portable code. 

	Nonetheless, a variety of examples are presented here- from the 
	extremely simple, to cooperative and liimted-preemptive multitasking 
	systems, all of which are examples are representative of real-world 
	systems that I've either written the firmware for, or have seen in my 
	experience. 
	
	\section SUPER1 The simplest loop 

	Let's start with the simplest embedded system design possible - an 
	infinite loop that performs a single task repeatedly: 

	\code 
	int main() 
	{ 
		while(1) 
		{ 
			Do_Something(); 
		}
	} 
	\endcode
	
	Here, the code inside the loop will run a single function forever and 
	ever. Not much to it, is there? But you might be surprised at just how 
	much embedded system firmware is implemented using essentially the same 
	mechanism - there isn't anything wrong with that, but it's just not that 
	interesting. 

	While the execution timeline for this program is equally boring, for the 
	sake of completeness it would look like this: 

	Despite its simplicity we can see the beginnings of some core OS 
	concepts. Here, the while(1) statement can be logically seen as the he 
	operating system kernel - this one control statement determines what 
	tasks can run in the system, and defines the constraints that could 
	modify their execution. But at the end of the day, that's a big part of 
	what a kernel is - a mechanism that controls the execution of 
	application code. 

	The second concept here is the task. This is application code provided 
	by the user to perform some useful purpose in a system. In this case 
	Do_something() represents that task - it could be monitoring blood 
	pressure, reading a sensor and writing its data to a terminal, or 
	playing an MP3; anything you can think of for an embedded system to do. 
	A simple round-robin multi-tasking system can be built off of this 
	example by simply adding additional tasks in sequence in the main 
	while-loop. Note that in this example the CPU is always busy running 
	tasks - at no time is the CPU idle, meaning that it is likely burning a 
	lot of power. 

	While we conceptually have two separate pieces of code involved here (an 
	operating system kernel and a set of running tasks), they are not 
	logically separate. The OS code is indistinguishable from the 
	application. It's like a single-celled organism - everything is crammed 
	together within the walls of an indivisible unit; and specialized to 
	perform its given function relying solely on instinct. 

	\section SUPER2 Interrupt-Driven Super-loop 

	In the previous example, we had a system without any way to control the 
	execution of the task- it just runs forever. There's no way to control 
	when the task can (or more importantly can't) run, which greatly limits 
	the usefulness of the system. Say you only want your task to run every 
	100 miliseconds - in the previous code, you have to add a hard-coded 
	delay at the end of your task's execution to ensure your code runs only 
	when it should. 

	Fortunately, there is a much more elegant way to do this. In this 
	example, we introduce the concept of the synchronization object. A 
	Synchronization object is some data structure which works within the 
	bounds of the operating system to tell tasks when they can run, and in 
	many cases includes special data unique to the synchronization event. 
	There are a whole family of synchronization objects, which we'll get 
	into later. In this example, we make use of the simplest synchronization 
	primitive - the global flag. 

	With the addition of synchronization brings the addition of event-driven 
	systems. If you're programming a microcontroller system, you generally 
	have scores of peripherals available to you - timers, GPIOs, ADCs, 
	UARTs, ethernet, USB, etc. All of which can be configured to provide a 
	stimulus to your system by means of interrupts. This stimulus gives us 
	the ability not only to program our micros to do_something(), but to 
	do_something() if-and-only-if a corresponding trigger has occurred. 

	The following concepts are shown in the example below: 

	\code 
	volatile K_BOOL something_to_do = false; 

	__interrupt__ My_Interrupt_Source(void) 
	{ 
		something_to_do = true; 
	} 

	int main() 
	{ 
		while(1) 
		{ 
			if( something_to_do ) 
			{ 
				Do_something(); 
				something_to_do = false; 
			} 
			else 
			{ 
				Idle(); 
			}
		}
	}
	\endcode
	
	So there you have it - an event driven system which uses a global 
	variable to synchronize the execution of our task based on the 
	occurrence of an interrupt. It's still just a bare-metal, 
	OS-baked-into-the-aplication system, but it's introduced a whole bunch 
	of added complexity (and control!) into the system. 

	The first thing to notice in the source is that the global variable, 
	something_to_do, is used as a synchronization object. When an interrupt 
	occurs from some external event, triggering the My_Interrupt_Source() 
	ISR, program flow in main() is interrupted, the interrupt handler is 
	run, and something_to_do is set to true, letting us know that when we 
	get back to main(), that we should run our Do_something() task. 

	Another new concept at play here is that of the idle function. In 
	general, when running an event driven system, there are times when the 
	CPU has no application tasks to run. In order to minimize power 
	consumption, CPUs usually contain instructions or registers that can be 
	set up to disable non-essential subsets of the system when there's 
	nothing to do. In general, the sleeping system can be re-activated 
	quickly as a result of an interrupt or other external stimulus, allowing 
	normal processing to resume. 

	Now, we could just call Do_something() from the interrupt itself - but 
	that's generally not a great solution. In general, the more time we 
	spend inside an interrupt, the more time we spend with at least some 
	interrupts disabled. As a result, we end up with interrupt latency. Now, 
	in this system, with only one interrupt source and only one task this 
	might not be a big deal, but say that Do_something() takes several 
	seconds to complete, and in that time several other interrupts occur 
	from other sources. While executing in our long-running interrupt, no
	other interrupts can be processed - in many cases, if two interrupts of 
	the same type occur before the first is processed, one of these 
	interrupt events will be lost. This can be utterly disastrous in a 
	real-time system and should be avoided at all costs. As a result, it's 
	generally preferable to use synchronization objects whenever possible to 
	defer processing outside of the ISR. 

	Another OS concept that is implicitly introduced in this example is that 
	of task priority. When an interrupt occurs, the normal execution of code 
	in main() is preempted: control is swapped over to the ISR (which runs 
	to completion), and then control is given back to main() where it left 
	off. The very fact that interrupts take precedence over what's running 
	shows that main is conceptually a "low-priority" task, and that all ISRs 
	are "high-priority" tasks. In this example, our "high-priority" task is 
	setting a variable to tell our "low-priority" task that it can do 
	something useful. We will investigate the concept of task priority 
	further in the next example. 

	Preemption is another key principle in embedded systems. This is the 
	notion that whatever the CPU is doing when an interrupt occurs, it 
	should stop, cache its current state (referred to as its context), and 
	allow the high-priority event to be processed. The context of the 
	previous task is then restored its state before the interrupt, and 
	resumes processing. We'll come back to preemption frequently, since the 
	concept comes up frequently in RTOS-based systems. 
	
	\section SUPER3 Cooperative multi-tasking 
	
	Our next example takes the previous example one step 
	further by introducing cooperative multi-tasking: 

	\code
	// Bitfield values used to represent three distinct tasks 
	#define TASK_1_EVENT (0x01) 
	#define TASK_2_EVENT (0x02) 
	#define TASK_3_EVENT (0x04) 

	volatile K_UCHAR event_flags = 0; 

	// Interrupt sources used to trigger event execution 
	
	__interrupt__  My_Interrupt_1(void) 
	{ 
		event_flags |= TASK_1_EVENT; 
	}
	
	__interrupt__ My_Interrupt_2(void) 
	{ 
		event_flags |= TASK_2_EVENT; 
	} 
	
	__interrupt__ My_Interrupt_3(void) 
	{ 
		event_flags |= TASK_3_EVENT; 
	} 

	// Main tasks 
	int main(void) 
	{ 
		while(1) 
		{ 
			while(event_flags) 
			{ 
				if( event_flags & TASK_1_EVENT) 
				{ 
					Do_Task_1(); 
					event_flags &= ~TASK_1_EVENT; 
				} else if( event_flags & TASK_2_EVENT) { 
					Do_Task_2(); 
					event_flags &= ~TASK_2_EVENT; 
				} else if( event_flags & TASK_3_EVENT) { 
					Do_Task_3(); 
					event_flags &= ~TASK_3_EVENT; 
				}
			} 
			Idle(); 
		} 
	} 
	
	\endcode
	
	This system is very similar to what we had before - however the 
	differences are worth discussing. First, we have stimulus from multiple 
	interrupt sources: each ISR is responsible for setting a single bit in 
	our global event flag, which is then used to control execution of 
	individual tasks from within main(). 

	Next, we can see that tasks are explicitly given priorities inside the 
	main loop based on the logic of the if/else if structure. As long as
	there is something set in the event flag, we will always try to execute 
	Task1 first, and only when Task1 isn't set will we attempt to execute 
	Task2, and then Task 3. This added logic provides the notion of 
	priority. However, because each of these tasks exist within the same 
	context (they're just different functions called from our main control 
	loop), we don't have the same notion of preemption that we have when 
	dealing with interrupts. 

	That means that even through we may be running Task2 and an event flag 
	for Task1 is set by an interrupt, the CPU still has to finish processing 
	Task2 to completion before Task1 can be run. And that's why this kind of 
	scheduling is referred to ascooperative multitasking: we can have as 
	many tasks as we want, but unless they cooperate by means of returning 
	back to main, the system can end up with high-priority tasks getting 
	starved for CPU time by lower-priority, long-running tasks.

	This is one of the more popular Os-baked-into-the-application 
	approaches, and is widely used in a variety of real-time embedded 
	systems. 

	\section SUPER4 Hybrid cooperative/preemptive multi-tasking 

	The final variation on the superloop design utilizes software-triggered 
	interrupts to simulate a hybrid cooperative/preemptive multitasking 
	system. Consider the example code below. 

	\code 
	// Bitfields used to represent high-priority tasks.	 Tasks in this group 
	// can preempt tasks in the group below - but not eachother.
	#define HP_TASK_1		 (0x01)
	#define HP_TASK_2		 (0x02)

	volatile K_UCHAR hp_tasks = 0;

	// Bitfields used to represent low-priority tasks.
	#define LP_TASK_1		 (0x01)
	#define LP_TASK_2		 (0x02)

	volatile K_UCHAR lp_tasks = 0;

	// Interrupt sources, used to trigger both high and low priority tasks.
	__interrupt__ System_Interrupt_1(void)
	{
		// Set any of the other tasks from here...
		hp_tasks |= HP_TASK_1;
		// Trigger the SWI that calls the High_Priority_Tasks interrupt handler
		SWI();
	}

	__interrupt__ System_Interrupt_n...(void)
	{
		// Set any of the other tasks from here...
	}


	// Interrupt handler that is used to implement the high-priority event context
	__interrupt__ High_Priority_Tasks(void)
	{
		// Enabled every interrupt except this one
		Disable_My_Interrupt();
		Enable_Interrupts();
		while( hp_tasks)
		{
			if( hp_tasks & HP_TASK_1)
			{
				HP_Task1();
				hp_tasks &= ~HP_TASK_1;
			}
			else if (hp_tasks & HP_TASK_2)
			{
				HP_Task2();
				hp_tasks &= ~HP_TASK_2;
			}
		}
		Restore_Interrupts();
		Enable_My_Interrupt();
	}

	// Main loop, used to implement the low-priority events
	int main(void)
	{
		// Set the function to run when a SWI is triggered
		Set_SWI(High_Priority_Tasks);
		
		// Run our super-loop
		while(1)
		{
			while (lp_tasks)
			{
				if (lp_tasks & LP_TASK_1)
				{
					LP_Task1();
					lp_tasks &= ~LP_TASK_1;
				}
				else if (lp_tasks & LP_TASK_2)
				{
					LP_Task2();
					lp_tasks &= ~LP_TASK_2;
				}
			}
			Idle();
		}
	}
	\endcode
	
	In this example, High_Priority_Tasks() can be triggered at any time as a 
	result of a software interrupt (SWI),. When a high-priority event is 
	set, the code that sets the event calls the SWI as well, which instantly 
	preempts whatever is happening in main, switching to the high-priority 
	interrupt handler. If the CPU is executing in an interrupt handler 
	already, the current ISR completes, at which point control is given to 
	the high priority interrupt handler. 

	Once inside the HP ISR, all interrupts (except the software interrupt) 
	are re-enabled, which allows this interrupt to be preempted by other 
	interrupt sources, which is called interrupt nesting. As a result, we 
	end up with two distinct execution contexts (main and 
	HighPriorityTasks()), in which all tasks in the high-priority group are 
	guaranteed to preempt main() tasks, and will run to completion before 
	returning control back to tasks in main(). This is a very basic 
	preemptive multitasking scenario, approximating a "real" RTOS system 
	with two threads of different priorities. 

	\section SUPER5 Problems with superloops 

	As mentioned earlier, a lot of real-world systems are implemented using 
	a superloop design; and while they are simple to understand due to the 
	limited and obvious control logic involved, they are not without their 
	problems. 

	<b>Hidden Costs</b> 

	It's difficult to calculate the overhead of the superloop and the code 
	required to implement workarounds for blocking calls, scheduling, and 
	preemption. There's a cost in both the logic used to implement 
	workarounds (usually involving state machines), as well as a cost to 
	maintainability that comes with breaking up into chunks based on 
	execution time instead of logical operations. In moderate firmware 
	systems, this size cost can exceed the overhead of a reasonably 
	well-featured RTOS, and the deficit in maintainability is something that 
	is measurable in terms of lost productivity through debugging and 
	profiling. 

	<b>Tightly-coupled code</b> 

	Because the control logic is integrated so closely with the application 
	logic, a lot of care must be taken not to compromise the separation 
	between application and system code. The timing loops, state machines, 
	and architecture-specific control mechanisms used to avoid (or simulate) 
	preemption can all contribute to the problem. As a result, a lot of 
	superloop code ends up being difficult to port without effectively 
	simulating or replicating the underlying system for which the 
	application was written. Abstraction layers can mitigate the risks, but 
	a lot of care should be taken to fully decouple the application code 
	from the system code. 

	<b>No blocking calls</b> 

	In a super-loop environment, there's no such thing as a blocking call or 
	blocking objects. Tasks cannot stop mid-execution for event-driven I/O 
	from other contexts - they must always run to completion. If 
	busy-waiting and polling are used as a substitute, it increases latency 
	and wastes cycles. As a result, extra code complexity is often times 
	necessary to work-around this lack of blocking objects, often times 
	through implementing additional state machines. In a large enough 
	system, the added overhead in code size and cycles can add up. 

	<b>Difficult to guarantee responsiveness</b> 

	Without multiple levels of priority, it may be difficult to guarantee a 
	certain degree of real-time responsiveness without added profiling and 
	tweaking. The latency of a given task in a priority-based cooperative 
	multitasking system is the length of the longest task. Care must be 
	taken to break tasks up into appropriate sized chunks in order to ensure 
	that higher-priority tasks can run in a timely fashion - a manual 
	process that must be repeated as new tasks are added in the system. Once 
	again, this adds extra complexity that makes code larger, more difficult 
	to understand and maintain due to the artificial subdivision of tasks 
	into time-based components. 

	<b>Limited preemption capability</b> 

	As shown in the example code, the way to gain preemption in a superloop 
	is through the use of nested interrupts. While this isn't unwiedly for 
	two levels of priority, adding more levels beyond this is becomes 
	complicated. In this case, it becomes necessary to track interrupt 
	nesting manually, and separate sets of tasks that can run within given 
	priority loops - and deadlock becomes more difficult to avoid. 
*/
/*!	 
	\page OVERVIEW Mark3 Overview 

	\section OVERVIEW0 Intro
	
	The following section details the overall design of Mark3, the goals 
	I've set out to achieve, the features that I've intended to provide, as 
	well as an introduction to the programming concepts used to make it 
	happen. 

	\section OVERVIEW1 Features
	
	Mark3 is a fully-featured real-time kernel, and is 
	feature-competitive with other open-source and commercial RTOS's in the 
	embedded arena. 
	
	The key features of this RTOS are:	  
	- Flexible Scheduler 
	  - Unlimited number of threads with 8 priority levels 
	  - Unlimited threads per priority level 
	  - Round-robin scheduling for threads at each priority level	 
	  - Time quantum scheduling for each thread in a given priority level 
	  .
	- Configurable stacks for each Thread 
	- Resource protection: 
	  - Integrated mutual-exclusion semaphores (Mutex) 
	  - Priority-inheritance on Mutex objects to prevent priority inversion 
	  .
	- Synchronization Objects 
	  - Binary and counting Semaphore to coordinate thread execution
	  - Event flags with 16-bit bitfields for complex thread synchronization
	  .
	- Efficient Timers
	  - The RTOS is tickless, the OS only wakes up when a timer expires, not
		at a regular interval
	  - One-shot and periodic timers with event callbacks
	  - Timers are high-precision and long-counting
		(about 68000 seconds when used with a 16us resolution timer)
	  .		 
	- Driver API 
	  - A hardware abstraction layer is provided to simplify driver development 
	  .
	- Robust Interprocess Communications 
	  - Threadsafe global Message pool and configurable message queues
	  .
	.
	
	\section OVERVIEW2 Design Goals
	
	<b>Lightweight</b> 

	Mark3 can be configured to have an extremely low static memory 
	footprint. Each thread is defined with its own stack, and each thread 
	structure can be configured to take as little as 26 bytes of RAM. 
	The complete Mark3 kernel with all features, setup code, a serial
	driver, and the Mark3 protocol libraries comes in at under 9K of 
	code space and 1K of RAM on atmel AVR. 

	<b>Modular</b> 
	
	Each system feature can be enabled or disabled by modifying the kernel 
	configuration header file. Include what you want, and ignore the rest to 
	save code space and RAM. 
	
	<b>Easily Portable</b> 

	Mark3 should be portable to a variety of 8, 16 and 32 bit architectures 
	without MMUs. Porting the OS to a new architecture is relatively 
	straightforward, requiring only device-specific implementations for the 
	lowest-level operations such as context switching and timer setup. 

	<b>Easy To Use</b>
	
	Mark3 is small by design - which gives it the advantage that it's also 
	easy to develop for. This manual, the code itself, and the Doxygen 
	documentation in the code provide ample documentation to get you up to 
	speed quickly. Because you get to see the source, there's nothing left 
	to assumption. 
	
	<b>Simple to Understand</b> 
	
	Not only is the Mark3 API rigorously documented (hey - that's what this 
	book is for!), but the architecture and naming conventions are intuitive -
	it's easy to figure out where code lives, and how it works. Individual 
	modules are small due to the "one feature per file" rule used in 
	development. This makes Mark3 an ideal platform for learning about 
	aspects of RTOS design. 
*/
/*!
	\page START Getting Started 

	\section START0 Kernel Setup
	
	This section details the process of defining threads, initializing the 
	kernel, and adding threads to the scheduler.
	
	If you're at all familiar with real-time operating systems, then these 
	setup and initialization steps should be familiar.	I've tried very hard
	to ensure that as much of the heavy lifting is hidden from the user, so 
	that only the bare minimum of calls are required to get things started.
	
	The examples presented in this chapter are real, working examples taken
	from the ATmega328p port.
	
	First, you'll need to create the necessary data structures and functions 
	for the threads:
	 -# Create a Thread object for all of the "root" or "initial" tasks.
	 -# Allocate stacks for each of the Threads
	 -# Define an entry-point function for each Thread
	 
	This is shown in the example code below:
	 
	\code	 
	//---------------------------------------------------------------------------
	#include "thread.h"
	#include "kernel.h"
	
	//1) Create a thread object for all of the "root" or "initial" tasks
	static Thread AppThread;
	static Thread IdleThread;
	
	//2) Allocate stacks for each thread
	#define STACK_SIZE_APP		(192)
	#define STACK_SIZE_IDLE		(128)
	
	static K_UCHAR aucAppStack[STACK_SIZE_APP];
	static K_UCHAR aucIdleStack[STACK_SIZE_IDLE];
	
	//3) Define entry point functions for each thread
	void AppThread(void);
	void IdleThread(void);
	
	\endcode
	
	Next, we'll need to add the required kernel initialization code to main.  
	This consists of running the Kernel's init routine, initializing all of 
	the threads we defined, adding the threads to the scheduler, and finally
	calling Kernel::Start(), which transfers control of the system to the
	RTOS.
	
	These steps are illustrated in the following example.
	
	\code
	
	int main(void)
	{		 
		//1) Initialize the kernel prior to use
		Kernel::Init();						   //!< MUST be before other kernel ops
		
		//2) Initialize all of the threads we've defined
		AppThread.Init(	   aucAppStack,		   //!< Pointer to the stack
						STACK_SIZE_APP,		   //!< Size of the stack
						1,					  //!< Thread priority						  
						(void*)AppEntry,	//!< Entry function
						NULL );				//!< Entry function argument

		IdleThread.Init( aucIdleStack,		  //!< Pointer to the stack
						 STACK_SIZE_IDLE,	 //!< Size of the stack
						 0,					   //!< Thread priority		   
						 4,					   //!< Thread quantum
						 (void*)IdleEntry,	  //!< Entry function
						 NULL );			//!< Entry function argument
		
		//3) Add the threads to the scheduler
		AppThread.Start();					  //!< Schedule the threads
		IdleThread.Start();
		
		//4) Give control of the system to the kernel
		Kernel::Start();					//!< Start the kernel!
	}	 
	\endcode
	
	Not much to it, is there?  
	There are a few noteworthy points in this code, though.
	
	In order for the kernel to work properly, a system must always contain 
	an idle thread; that is, a thread at priority level 0 that never
	blocks.	 This thread is responsible for performing any of the low-level
	power management on the CPU in order to maximize battery life in an
	embedded device.  The idle thread must also never block, and it 
	must never exit.  Either of these operations will cause undefined
	behavior in the system.
	
	The App thread is at a priority level greater-than 0.  This ensures that
	as long as the App thread has something useful to do, it will be given
	control of the CPU.	 In this case, if the app thread blocks, control
	will be given back to the Idle thread, which will put the CPU into
	a power-saving mode until an interrupt occurs.
	
	Stack sizes must be large enough to accommodate not only the requirements
	of the threads, but also the requirements of interrupts - up to the 
	maximum interrupt-nesting level used.  Stack overflows are super-easy
	to run into in an embedded system; if you encounter strange and
	unexplained behavior in your code, chances are good that one of your
	threads is blowing its stack.

	\section THREADS Threads
   
	Mark3 Threads act as independent tasks in the system.  While they share 
	the same address-space, global data, device-drivers, and system 
	peripherals, each thread has its own set of CPU registers and stack,
	collectively known as the thread's <b>context</b>.	The context
	is what allows the RTOS kernel to rapidly switch between threads 
	at a high rate, giving the illusion that multiple things are happening
	in a system, when really, only one thread is executing at a time.
   
	\subsection TH_SETUP Thread Setup
	
	Each instance of the Thread class represents a thread, its stack,
	its CPU context, and all of the state and metadata maintained by the 
	kernel.	 Before a Thread will be scheduled to run, it must first be
	initialized with the necessary configuration data.
	
	The Init function gives the user the opportunity to set the stack,
	stack size, thread priority, entry-point function, entry-function
	argument, and round-robin time quantum:
	
	Thread stacks are pointers to blobs of memory (usually K_CHAR arrays)
	carved out of the system's address space.  Each thread must have a 
	stack defined that's large enough to handle not only the requirements
	of local variables in the thread's code path, but also the maximum 
	depth of the ISR stack.	 
	
	Priorities should be chosen carefully such that the shortest tasks
	with the most strict determinism requirements are executed first -
	and are thus located in the highest priorities.	 Tasks that take the
	longest to execute (and require the least degree of responsiveness)
	must occupy the lower thread priorities.  The idle thread must be 
	the only thread occupying the lowest priority level.
	
	The thread quantum only aplies when there are multiple threads in
	the ready queue at the same priority level.	 This interval is used
	to kick-off a timer that will cycle execution between the threads
	in the priority list so that they each get a fair chance to execute.
	
	The entry function is the function that the kernel calls first when
	the thread instance is first started.  Entry functions have at most
	one argument - a pointer to a data-object specified by the user 
	during initialization.
	
	An example thread initailization is shown below:
	
	\code 
	Thread clMyThread;
	K_UCHAR aucStack[192];
	
	void AppEntry(void)
	{
		while(1)
		{
			// Do something!
		}
	}
	
	...
	{	 
		clMyThread.Init(aucStack,			//!< Pointer to the stack to use by this thread
						192,				//!< Size of the stack in bytes
						1,					  //!< Thread priority (0 = idle, 7 = max)
						4,					  //!< Thread quantum (in milliseconds), used by RR scheduler
						(void*)AppEntry,	//!< Function where the thread starts executing
						NULL );				//!< Argument passed into the entry function
						
	}
	
	\endcode
	
	Once a thread has been initialized, it can be added to the scheduler by calling:
	
	\code
	
	clMyThread.Start();
	
	\endcode
	
	The thread will be placed into the Scheduler's queue at the designated 
	priority, where it will wait its turn for execution.
	
	\subsection TH_ENTRY Entry Functions
	
	Mark3 Threads should not run-to-completion - they should execute as 
	infinite loops that perform a series of tasks, appropriately partitioned
	to provide the responsiveness characteristics desired in the system.
	
	The most basic Thread loop is shown below:
	  
	\code
	
	void Thread( void *param )
	{
		while(1)
		{
			// Do Something
		}	 
	}
	
	\endcode
   
	Threads can interact with eachother in the system by means of 
	synchronization objects (Semaphore), mutual-exclusion objects (Mutex),
	Inter-process messaging (MessageQueue), and timers (Timer).
	   
	Threads can suspend their own execution for a predetermined period of
	time by using the static Thread::Sleep() method.  Calling this will
	block the Thread's executin until the amount of time specified has 
	ellapsed.  Upon expiry, the thread will be placed back into the ready
	queue for its priority level, where it awaits its next turn to run.
	 
	\section TIMERS Timers
	
	Timer objects are used to trigger callback events periodic or on a one-shot 
	(alarm) basis.	
	
	While extremely simple to use, they provide one of the most powerful
	execution contexts in the system.  The timer callbacks execute from within
	the timer callback ISR in an interrupt-enabled context.	 As such,
	timer callbacks are considered higher-priority than any thread in the system,
	but lower priority than other interrupts.  Care must be taken to ensure
	that timer callbacks execute as quickly as possible to minimize the 
	impact of processing on the throughput of tasks in the system.	
	Wherever possible, heavy-lifting should be deferred to the threads
	by way of semaphores or messages.
	
	Below is an example showing how to start a periodic system timer 
	which will trigger every second:
	
	\code 
	{
		Timer clTimer;
		clTimer.Init();
		
		clTimer.Start( 1000,			  //!< Time in ms to use as the interval
					 1,					  //!< 1 = repeat, 0 = one-shot
					 MyCallback,		  //!< Function that gets called on expiry
					 (void*)&my_data );	  //!< Data passed to the timer callback
					 
		... // Keep doing work in the thread
	}
	
	// Callback function, executed from the timer-expiry context.
	void MyCallBack( Thread *pclOwner_, void *pvData_ )
	{
		LED.Flash(); // Flash an LED.
	}

	\endcode
	
	\section SEM Semaphores

	Semaphores are used to synchronized execution of threads based on the 
	availability (and quantity) of application-specific resources in the
	system.	 They are extremely useful for solving producer-consumer 
	problems, and are the method-of-choice for creating efficient, low
	latency systems, where ISRs post semaphores that are handled
	from within the context of individual threads.	(Yes, Semaphores
	can be posted - but not pended - from the interrupt context).
	
	The following is an example of the producer-consumer usage of a binary
	semaphore:
	
	\code 
	
	Semaphore clSemaphore; // Declare a semaphore shared between a producer and a consumer thread.
	
	void Producer()
	{
		clSemaphore.Init(0, 1);	 //!< Intialize as a binary semaphore, with an initial value of 0, maximum value of 1.
		while(1)
		{
			// Do some work, create something to be consumed
			
			// Post a semaphore, allowing another thread to consume the data
			clSemaphore.Post();
		}
	}
	
	void Consumer()
	{
		// Assumes semaphore initialized before use...
		While(1)
		{
			// Wait for new data from the producer thread
			clSemaphore.Pend();
			
			// Consume the data!
		}
	}	 
	\endcode
	
	And an example of using semaphores from the ISR context to perform event-
	driven processing.
	
	\code 
	
	Semaphore clSemaphore;
	
	__interrupt__ MyISR()
	{
		clSemaphore.Post(); // Post the interrupt.	Lightweight when uncontested.
	}
	
	void MyThread()
	{
		clSemaphore.Init(0, 1); // Ensure this is initialized before the MyISR interrupt is enabled.
		while(1)
		{
			// Wait until we get notification from the interrupt
			clSemaphore.Pend();
			
			// Interrupt has fired, do the necessary work in this thread's context
			HeavyLifting();
		}	 
	}
	
	\endcode

	\section MUT Mutexes

	Mutexes (Mutual exclusion objects) are provided as a means of creating 
	"protected sections" around a particular resource, allowing for access 
	of these objects to be serialized. Only one thread can hold the mutex at 
	a time - other threads have to wait until the region is released by the 
	owner thread before they can take their turn operating on the protected 
	resource. Note that mutexes can only be owned by threads - they are not 
	available to other contexts (i.e. interrupts). Calling the mutex APIs 
	from an interrupt will cause catastrophic system failures. 
		
	Note that these objects are also not recursive- that is, the owner 
	thread can not attempt to claim a mutex more than once. 
		
	Prioritiy inheritence is provided with these objects as a means to 
	avoid prioritiy inversions. Whenever a thread at a priority than the 
	mutex owner blocks on a mutex, the priority of the current thread is 
	boosted to the highest-priority waiter to ensure that other tasks at 
	intermediate priorities cannot artificically prevent progress from being 
	made. 
		
	Mutex objects are very easy to use, as there are only three operations 
	supported: Initialize, Claim and Release. An example is shown below. 
	
	\code 
	
	Mutex clMutex;	  // Create a mutex globally.
	
	void Init()
	{
		// Initialize the mutex before use.
		clMutex.Init();
	}
	
	// Some function called from a thread
	void Thread1Function()
	{
		clMutex.Claim();
	
		// Once the mutex is owned, no other thread can 
		// enter a block protect by the same mutex
	
		my_protected_resource.do_something();
		my_protected_resource.do_something_else();
	
		clMutex.Release();
	}
	
	// Some function called from another thread
	void Thread2Function()
	{
		clMutex.Claim();
				
		// Once the mutex is owned, no other thread can 
		// enter a block protect by the same mutex

		my_protected_resource.do_something();
		my_protected_resource.do_different_things();
		
		clMutex.Release();
	}
	
	\endcode 
	
	\section EVF Event Flags

	Event Flags are another synchronization object, conceptually similar
	to a semaphore.

	Unlike a semaphore, however, the condition on which threads are unblocked
	is determined by a more complex set of rules.  Each Event Flag object
	contains a 16-bit field, and threads block, waiting for combinations of
	bits within this field to become set.

	A thread can wait on any pattern of bits from this field to be set, and
	any number of threads can wait on any number of different patterns.	 Threads
	can wait on a single bit, multiple bits, or bits from within a subset of
	bits within the field.

	As a result, setting a single value in the flag can result in any number of
	threads becoming unblocked simultaneously.	This mechanism is extremely
	powerful, allowing for all sorts of complex, yet efficient, thread
	synchronization schemes that can be created using a single shared object.

	Note that Event Flags can be set from interrupts, but you cannot wait on
	an event flag from within an interrupt.

	Examples demonstrating the use of event flags are shown below.

	\code

		// Simple example showing a thread blocking on a multiple bits in the
		// fields within an event flag.

		EventFlag clEventFlag;

		int main()
		{
			...
			clEventFlag.Init(); // Initialize event flag prior to use
			...
		}

		void MyInterrupt()
		{
			// Some interrupt corresponds to event 0x0020
			clEventFlag.Set(0x0020);
		}

		void MyThreadFunc()
		{
			...
			while(1)
			{
				...
				K_USHORT usWakeCondition;

				// Allow this thread to block on multiple flags
				usWakeCondition = clEventFlag.Wait(0x00FF, EVENT_FLAG_ANY);

				// Clear the event condition that caused the thread to wake (in this case,
				// usWakeCondtion will equal 0x20 when triggered from the interrupt above)
				clEventFlag.Clear(usWakeCondition);

				// <do something>
			}
		}

	\endcode

	\section MSG Messages
	
	Sending messages between threads is the key means of synchronizing 
	access to data, and the primary mechanism to perform asynchronous data 
	processing operations. 
	
	Sending a message consists of the following operations:
	
	- Obtain a Message object from the global message pool
	- Set the message data and event fields
	- Send the message to the destination message queue
	.
	
	While receiving a message consists of the following steps:
	
	- Wait for a messages in the destination message queue
	- Process the message data
	- Return the message back to the global message pool
	.

	These operations, and the various data objects involved are discussed in 
	more detail in the following section. 

	\subsection MSGO Message Objects
	
	Message objects are used to communicate arbitrary data between threads 
	in a safe and synchronous way. 
		
	The message object consists of an event code field and a data field. 
	The event code is used to provide context to the message object, while 
	the data field (essentially a void * data pointer) is used to provide a 
	payload of data corresponding to the particular event. 
		
	Access to these fields is marshalled by accessors - the transmitting 
	thread uses the SetData() and SetCode() methods to seed the data, while 
	the receiving thread uses the GetData() and GetCode() methods to 
	retrieve it. 
		
	By providing the data as a void data pointer instead of a fixed-size 
	message, we achieve an unprecedented measure of simplicity and 
	flexibility. Data can be either statically or dynamically allocated, and 
	sized appropriately for the event without having to format and reformat 
	data by both sending and receiving threads. The choices here are left to 
	the user - and the kernel doesn't get in the way of efficiency. 

	It is worth noting that you can send messages to message queues from
	within ISR context.	 This helps maintain consistency, since the same
	APIs can be used to provide event-driven programming facilities 
	throughout the whole of the OS.
	
	\subsection MSGP Global Message Pool
	
	To maintain efficiency in the messaging system (and to prevent 
	over-allocation of data), a global pool of message objects is provided. 
	The size of this message pool is specified in the implementation, and 
	can be adjusted depending on the requirements of the target application 
	as a compile-time option. 
		
	Allocating a message from the message pool is as simple as calling the 
	GlobalMessagePool::Pop() Method. 
		
	Messages are returned back to the GlobalMessagePool::Push() method once 
	the message contents are no longer required. 
		
	One must be careful to ensure that discarded messages always are 
	returned to the pool, otherwise a resource leak can occur, which may 
	cripple the operating system's ability to pass data between threads. 
	
	\subsection MSGQ Message Queues
	
	Message objects specify data with context, but do not specify where the 
	messages will be sent. For this purpose we have a MessageQueue object. 
	Sending an object to a message queue involves calling the 
	MessageQueue::Send() method, passing in a pointer to the Message object 
	as an argument. 

	When a message is sent to the queue, the first thread blocked on the 
	queue (as a result of calling the MessageQueue Receive() method) will 
	wake up, with a pointer to the Message object returned. 

	It's worth noting that multiple threads can block on the same message 
	queue, providing a means for multiple threads to share work in parallel.

	\subsection MSGQS Messaging Example
	
	\code
	
	// Message queue object shared between threads
	MessageQueue clMsgQ;
	
	// Function that initializes the shared message queue
	void MsgQInit()
	{
		clMsgQ.Init();
	}
	
	// Function called by one thread to send message data to 
	// another
	void TxMessage()
	{	 
		// Get a message, initialize its data
		Message *pclMesg = GlobalMessagePool::Pop();
		
		pclMesg->SetCode(0xAB);
		pclMesg->SetData((void*)some_data);	   
		
		// Send the data to the message queue
		clMsgQ.Send(pclMesg);
	}
	
	// Function called in the other thread to block until
	// a message is received in the message queue.	  
	void RxMessage()
	{
		Message *pclMesg;
		
		// Block until we have a message in the queue
		pclMesg = clMsgQ.Receive();
		
		// Do something with the data once the message is received
		pclMesg->GetCode();
		
		// Free the message once we're done with it.
		GlobalMessagePool::Push(pclMesg);
	}
	\endcode
	
	\section SLP Sleep
	
	There are instances where it may be necessary for a thread to poll a 
	resource, or wait a specific amount of time before proceeding to operate 
	on a peripheral or volatile piece of data. 
		
	While the Timer object is generally a better choice for performing 
	time-sensitive operations (and certainly a better choice for periodic 
	operations), the Thread::Sleep() method provides a convenient (and 
	efficient) mechanism that allows for a thread to suspend its execution 
	for a specified interval. 
		
	Note that when a thread is sleeping it is blocked, during which other 
	threads can operate, or the system can enter its idle state. 
	
	\code
	int GetPeripheralData();
	{
		int value;
		// The hardware manual for a peripheral specifies that
		// the "foo()" method will result in data being generated
		// that can be captured using the "bar()" method.
		// However, the value only becomes valid after 10ms
		
		peripheral.foo();	 
		Thread::Sleep(10);	  // Wait 10ms for data to become valid
		value = peripheral.bar();
		return value;
	}
	\endcode 
	
	\section RR Round-Robin Quantum
	
	Threads at the same thread priority are scheduled using a round-robin
	scheme.	 Each thread is given a timeslice (which can be configured) 
	of which it shares time amongst ready threads in the group.	 Once 
	a thread's timeslice has expired, the next thread in the priority 
	group is chosen to run until its quantum has expired - the cycle
	continues over and over so long as each thread has work to be done.
	
	By default, the round-robin interval is set at 4ms.
	
	This value can be overridden by calling the thread's SetQuantum()
	with a new interval specified in milliseconds.
*/
/*!
    \page SCHEDPAGE Inside The Scheduler

    This section details the inner-working of the Mark3 Scheduler in detail.

    \section SAB A Bit About Threads

    Before we get started talking about the internals of the
    Mark3 scheduler, it's necessary to go over some background material - starting
    with: what is a thread, anyway?

    Let's look at a very basic CPU without any sort of RTOS, and without
    interrupts. When the CPU is powered up, the program counter is loaded with some
    default location, at which point the processor core will start executing
    instructions sequentially - running forever and ever according to whatever has
    been loaded into program memory. This single instance of a simple program
    sequence is the only thing that runs on the processor, and the execution of the
    program can be predicted entirely by looking at the CPU's current register
    state, its program, and any affected system memory (the CPU's "context").

    It's simple enough, and that's exactly the definition we have for a thread in
    an RTOS.

    Each thread contains an instance of a CPU's register context, its own stack,
    and any other bookkeeping information necessary to define the minimum unique
    execution state of a system at runtime. It is the job of a RTOS to multiplex
    the execution of multiple threads on a single physical CPU, thereby creating
    the illusion that many programs are being executed simultaneously. In reality
    there can only ever be one thread truly executing at any given moment on a CPU
    core, so it's up to the scheduler to set and enforce rules about what thread
    gets to run when, for how long, and under what conditions. As mentioned
    earlier, any system without an RTOS exeuctes as a single thread, so at least
    two threads are required for an RTOS to serve any useful purpose.

    Note that all of this information is is common to pretty well every RTOS in
    existence - the implementation details, including the scheduler rules, are all
    part of what differentiates one RTOS from another. THREAD STATES AND THREAD
    LISTS Since only one thread can run on a CPU at a time, the scheduler relies on
    thread information to make its decisions. Mark3's scheduler relies on a variety
    of such information, including: The thread's current priority Round-Robin
    execution quanta Whether or not the thread is blocked on a synchronization
    object, such as a mutex or semaphore Whether or not the thread is currently
    suspended The scheduler further uses this information to logically place each
    thread into 1 of 4 possible states: Ready - The thread is currently running
    Running - The thread is able to run Blocked - The thread cannot run until a
    system condition is met Stopped - The thread cannot run because its execution
    has been suspended In order to determine a thread's state, threads are placed
    in "buckets" corresponding to these states. Ready and running threads exist in
    the scheduler's buckets, blocked threads exist in a buckets belonging to the
    object they're blocked on, and stopped threads exist in a bucket of all stopped
    threads.

    In reality, the various buckets are just doubly-linked lists of Thread objects
    - implemented in something called the ThreadList class. To facilitate this, the
    Thread class directly inherits from the LinkListNode class, which contains the
    node pointers required to implement a doubly-linked list. As a result, Threads
    may be effortlessly moved from one state to another using efficient linked-list
    operations built into the ThreadList class.

    \section ABB About Blocking and Unblocking

    While many developers new to the concept of an RTOS assume that all threads in
    a system are entirely separate from eachother, the reality is that practical
    systems typically involve multiple threads working together, or at the very
    least sharing resources. In order to synchronize the execution of threads for
    that purpose, a number of synchronization primatives (blocking objects) are
    implemented to create specific sets of conditions under which threads can
    continue execution. The concept of "blocking" a thread until a specific
    condition is met is fundamental to understanding RTOS applications design, as
    well as any highly-multithreaded applications.

    Blocking objects and primatives provided by Mark3 include:
    - Semaphores (binary and counting)
    - Mutexes
    - Event Flags
    - Thread Sleep
    - Message Queues
    .

    Each of these objects inherit from the BlockingObject class, which itself
    contains a ThreadList object. This class contains methods to Block() a thread
    (remove it from the Scheduler's "Ready" or "Running" ThreadLists), as well as
    UnBlock() a thread (move a thread back to the "Ready" lists). This object
    handles transitioning threads from list-to-list (and state-to-state), as well
    as taking care of any other Scheduler bookkeeping required in the process.
    While each of the Blocking types implement a different condition, they are
    effectively variations on the same theme. Many simple Blocking objects are also
    used to build complex blocking objects - for instance, the Thread Sleep
    mechanism is essentially a binary semaphore and a timer object, while a message
    queue is a linked-list of message objects combined with a semaphore.

    \section TSA The Scheduling Alogrithm

    At this point we've covered the following concepts:
    - Threads
    - Thread States and Thread Lists
    - Blocking and Un-Blocking Threads
    .

    Thankfully, this is all the background required to understand how the Mark3
    Scheduler works. In technical terms, Mark3 implements "strict priority
    scheduling, with round-robin scheduling among threads in each priority group".
    In plain English, this boils down to a scheduler which follows a few simple
    rules:

    - Find the highest-priority "Ready" list that has at least one Threads.
    - If the first thread in that bucket is not the current thread, select it to run next
    - Otherwise, rotate the linked list, and choose the next thread in the list to run
    .

    Since context switching is one of the most common and frequent operation
    performed by an RTOS, this needs to be as fast and deterministic as possible.
    While the logic is simple, a lot of care must be put into optimizing the
    scheduler to achieve those goals. In the section below we discuss the
    optimization approaches taken in Mark3.

    There are a number of ways to find the highest-priority thread. The naive
    approach would be to simply iterate through the scheduler's array of
    ThreadLists from highest to lowest, stopping when the first non-empty list is
    found, such as in the following block of code:

    \code
    for (prio = num_prio - 1; prio >= 0; prio--)
    {
        if (thread_list[prio].get_head() != NULL)
        {
            break;
        }
    }
    \endcode

    While that would certainly work and be sufficient for a variety of systems,
    it's a non-deterministic approach (complexity O(n)) whose cost varies
    substantially based on how many priorities have to be evaluated. It's simple to
    read and understand, but it's non-optimal.

    Fortunatley, a functionally-equivalent and more deterministic approach can be implemented with a few tricks.

    In addition to maintaining an array of ThreadLists, Mark3 also maintains a
    bitmap (one bit per priority level) that indicates which thread lists have
    ready threads. This bitmap is maintained automatically by the ThreadList class,
    and is updated every time a thread is moved to/from the Scheduler's ready
    lists.

    By inspecting this bitmap using a technique to count the leading zero bits in
    the bitmap, we determine which threadlist to choose in fixed time.

    Now, to implement the leading-zeros check, this can once again be performed
    iteratively using bitshifts and compares (which isn't any more efficient than
    the raw list traversal), but it can also be evaluated using either a lookup
    table, or via a special CPU instruction to count the leading zeros in a value.
    In Mark3, we opt for the lookup-table approach since we have a limited number
    of priorities and not all supported CPU architectures support a count leading
    zero instruction. To achieve a balance between performance and memory use, we
    use a 4-bit lookup table (costing 16 bytes) to perform the lookup.

    (As a sidenote - this is actually a very common approach in OS schedulers. It's
    actually part of the reason why modern ARM cores implement a dedicated
    count-leading-zeros [CLZ] instruction!)

    With a 4-bit lookup table and an 8-bit priority-level bitmap, the priority
    check algorithm looks something like this:

    \code
    // Check the highest 4 priority levels, represented in the
    // upper 4 bits in the bitmap
    priority = priority_lookup_table[(priority_bitmap >> 4)];

    // priority is non-zero if we found something there
    if( priority )
    {
        // Add 4 because we were looking at the higher levels
        priority += 4;
    }
    else
    {
        // Nothing in the upper 4, look at the lowest 4 priority levels
        // represented by the lowest 4 bits in the bitmap
        priority = priority_lookup_table[priority_bitmap & 0x0F];
    }
    \endcode

    Deconstructing this algorithm, you can see that the priority lookup will have
    on O(1) complexity - and is extremely low-cost. This operation is thus fully
    deterministic and time bound - no matter how many threads are scheduled, the
    operation will always be time-bound to the most expensive of these two code
    paths. Even with only 8 priority levels, this is still much faster than
    iteratively checking the thread lists manually, compared with the previous
    example implementation.

    Once the priority level has been found, selecting the next thread to run is
    trivial, consisting of something like this:

    \code
    next_thread = thread_list[prio].get_head();
    \endcode

    In the case of the get_head() calls, this evaluates to an inline-load of the
    "head" pointer in the particular thread list. One important thing to take away
    from this analysis is that the scheduler is only responsible for selecting the
    next-to-run thread. In fact, these two operations are totally decoupled - no
    context switching is performed by the scheduler, and the scheduler isn't called
    from the context switch. The scheduler simply produces new "next thread" values
    that are consumed from within the context switch code.

    \section CRR Considerations For Round-Robin Scheduling

    One thing that isn't considered directly from the scheduler algorithm is the
    problem of dealing with multiple threads within a single priority group; all of
    the alorithms that have been explored above simply look at the firstThread in
    each group.

    Mark3 addresses this issue indirectly, using a software timer to manage
    round-robin scheduling, as follows. In some instances where the scheduler is
    run by the kernel directly (typically as a result of calling Thread::Yield()),
    the kernel will perfom an additional check after running the Scheduler to
    determine whether or there are multiple ready Threadsin the priority of the
    next ready thread. If there are multiple threads within that priority, the
    kernel adds a one-shot software timer which is programmed to expire at the next
    Thread's configured quantum. When this timer expires, the timer's callback
    function executes to perform two simple operations: "Pivot" the current
    Thread's priority list.

    Set a flag telling the kernel to trigger a Yield after exiting the main
    TimerScheduler processing loop Pivoting the thread list basically moves the
    head of a circular-linked-list to its next value, which in our case ensures
    that a new thread will be chosen the next time the scheduler is run (the
    scheduler only looks at the head node of the priority lists). And by calling
    Yield, the system forces the scheduler t run, a new round-robin software timer
    to be installed (if necssary), and triggers a context switch SWI to load the
    newly-chosen thread. Note that if the thread attached to the round-robin timer
    is pre-empted, the kernel will take steps to abort and invalidate that
    round-robin software timer, installing a new one tied to the next thread to run
    if necessary. Because the round-robin software timer is dynamically installed
    when there are multiple ready threads at the highest ready priority level,
    there is no CPU overhead with this feature unless that condition is met. The
    cost of round-robin scheduling is also fixed - no matter how many threads there
    are, and the cost is identical to any other one-shot software timer in the
    system.

    \section CSW Context Switching

    There's really not much to say about the actual context switch operation at a
    high level. Context switches are triggered whenever it has been determined that
    a new thread needs to be swapped into the CPU core when the scheduler is run.
    Mark3 implements also context switches as a call to a software interrupt - on
    AVR platforms, we typically use INT0 or INT2 for this (although any pin-change
    GPIO interrupt can be used), and on ARM we achieve this by triggering a PendSV
    exception.

    However, regardless of the architecture, the contex-switch ISR will perform the
    following three operations:
    - Save the current Thread's context to the current Thread stack
    - Make the "next to run" thread the "currently running" thread
    - Restore the context of the next Thread from the Thread stack
    .

    The code to implement the context switch is entirely architecture-specific, so
    it won't be discussed in detail here. It's almost always gory inline-assembly
    which is used to load and store various CPU registers, and is highly-optimized
    for speed. I will dive into how this imporant bit of code works (on ARM
    Cortex-M0+) in a separate whitepaper.

    \section PAT Putting It All Together

    In short, we can say that the Mark3 scheduler works as follows:

    - The scheduler is run whenever a Thread::Yield() is called by a user, as part of blocking calls, or whenever a new thread is started
    - The Mark3 scheduler is deterministic, selecting the next thread to run in fixed-time
    - The scheduler only chooses the next thread to run, the context switch SWI consumes that information to get that thread running
    - Where there are multiple ready threads in the highest populated priority level, a software timer is used to manage round-robin scheduling
    .

    While we've covered a lot of ground in this chapter, there's not a whole lot of code involved.
    However, the code that performs these operations is quite nuanced and subtle.
    If you're interested in seeing how this all works in practice, I suggest reading
    through the Mark3 source code (which is heavily annotated), and stepping through
    the code with a simulator/emulator.
*/
/*!
    \page PCM Porting Mark3 - An Example Using ARM Cortex-M0

    This document serves as both a real-world example of how Mark3 can be ported to
    new architectures, and as a practical reference for using the RTOS support
    functionality baked in modern ARM Cortex-M series microcontrollers. Most of
    this documentation here is taken directly from the source code found in the
    /kernel/cpu/cm0/ ports directory, with additional annotations to explain the
    port in more detail. Note that a familiarity with Cortex-M series parts will go
    a long way to understanding the subject matter presented, especially a basic
    understanding of the ARM CPU registers, exception models, and OS support
    features (PendSV, SysTick and SVC).

    Porting Mark3 to a new architecture consists of a few basic pieces; for
    developers familiar with the target architecture and the porting process, it's
    not a tremendously onerous endeavour to get Mark3 up-and-running somewhere new.
    For starters, all non-portable components are completely isolated in the
    source-tree under /embedded/kernel/<CPU>/<VARIANT>/<TOOLCHAIN>/, where <CPU> is
    the architecture, <VARIANT> is the vendor/part, and <TOOLCHAIN> is the compiler
    tool suite used to build the code.

    From within the specific port folder, a developer needs only implement a few
    classes and headers that define the port-specific behavior of Mark3:
    - KernelSWI (kernelswi.cpp/kernelswi.h) - Provides a maskable software-triggered interrupt used to perform context switching.
    - KernelTimer (kerneltimer.cpp/kerneltimer.h) - Provides either a fixed-frequency or programmable-interval timer, which triggers an interrupt on expiry. This is used for implementing round-robin scheduling, thread-sleeps, and generic software timers.
    - Profiler (kprofile.cpp/kprofile.h) - Contains code for runtime code-profiling.  This is optional and may be stubbed out if left unimplemented (we won't cover profiling timers here).
    - ThreadPort (threadport.cpp/threadport.h) - The meat-and-potatoes of the port code lives here. This class contains architecture/part-specific code used to initialize threads, implement critical-sections, perform context-switching, and start the kernel. Most of the time spent in this article focuses on the code found here.
    .

    Summarizing the above, these modules provide the following list of functionality:
    - Thread stack initialization
    - Kernel startup and first thread entry
    - Context switch and SWI
    - Kernel timers
    - Critical Sections
    .

    The implementation of each of these pieces will be analyzed in detail in the sections that follow.

    \section TSI Thread Stack Initialization

    Before a thread can be used, its stack must first be initialized to its default
    state. This default state ensures that when the thread is scheduled for the
    first time and its context restored, that it will cause the CPU to jump to the
    user's specified entry-point function.

    All of the platform independent thread setup is handled by the generic kernel
    code. However, since every CPU architecture has its own register set, and
    stacks different information as part of an interrupt/exception, we have to
    implement this thread setup code for each platform we want the kernel to
    support (Combination of Architecture + Variant + Toolchain).

    In the ARM Cortex-M0 architecture, the stack frame consists of the following information:

    a) Exception Stack Frame

    Contains the 8 registers which the ARM Cortex-M0 CPU automatically pushes to the stack when entering an exception.  The following registers are included (in stack'd order):
    \code
        [ XPSR ] <-- Highest address in context
        [ PC   ]
        [ LR   ]
        [ R12  ]
        [ R3   ]
        [ R2   ]
        [ R1   ]
        [ R0   ]
    \endcode

    XPSR – This is the CPU's status register. We need to set this to 0x01000000
    (the "T" bit), which indicates that the CPU is executing in “thumb” mode. Note
    that ARMv6m and ARMv7m processors only run thumb2 instructions, so an exception
    is liable to occur if this bit ever gets cleared.

    PC – Program Counter. This should be set with the initial entry point (function
    pointer) for that the user wishes to start executing with this thread.

    LR - The base link register. Normally, this register contains the return
    address of the calling function, which is where the CPU jumps when a function
    returns. However, our threads generally don't return (and if they do, they're
    placed into the stop state). As a result we can leave this as 0.

    The other registers in the stack frame are generic working registers, and have
    no special meaning, with the exception that R0 will hold the user's argument
    value passed into the entrypoint.

    b) Complimentary CPU Register Context
    \code
        [ R11   ]
        ...
        [ R4    ] <-- Lowest address in context
    \endcode

    These are the other general-purpose CPU registers that need to be backed
    up/restored on a context switch, but aren't stacked by default on a Cortex-M0
    exception. If there were any additional hardware registers to back up, then
    we'd also have to include them in this part of the context as well.

    As a result, these registers all need to be manually pushed to the stack on
    stack creation, and will need to be explicitly pushed and pop as part of a
    normal context switch.

    With this default exception state in mind, the following code is used to
    initialize a thread's stack for a Cortex-M0.

    \code
    void ThreadPort::InitStack(Thread *pclThread_)
    {
        K_ULONG *pulStack;
        K_ULONG *pulTemp;
        K_ULONG ulAddr;
        K_USHORT i;

        // Get the entrypoint for the thread
        ulAddr = (K_ULONG)(pclThread_->m_pfEntryPoint);

        // Get the top-of-stack pointer for the thread
        pulStack = (K_ULONG*)pclThread_->m_pwStackTop;

        // Initialize the stack to all FF's to aid in stack depth checking
        pulTemp = (K_ULONG*)pclThread_->m_pwStack;
        for (i = 0; i < pclThread_->m_usStackSize / sizeof(K_ULONG); i++)
        {
            pulTemp[i] = 0xFFFFFFFF;
        }

        PUSH_TO_STACK(pulStack, 0);             // Apply one word of padding

        //-- Simulated Exception Stack Frame --
        PUSH_TO_STACK(pulStack, 0x01000000);    // XSPR - set "T" bit for thumb-mode
        PUSH_TO_STACK(pulStack, ulAddr);        // PC
        PUSH_TO_STACK(pulStack, 0);             // LR
        PUSH_TO_STACK(pulStack, 0x12);
        PUSH_TO_STACK(pulStack, 0x3);
        PUSH_TO_STACK(pulStack, 0x2);
        PUSH_TO_STACK(pulStack, 0x1);
        PUSH_TO_STACK(pulStack, (K_ULONG)pclThread_->m_pvArg);    // R0 = argument

        //-- Simulated Manually-Stacked Registers --
        PUSH_TO_STACK(pulStack, 0x11);
        PUSH_TO_STACK(pulStack, 0x10);
        PUSH_TO_STACK(pulStack, 0x09);
        PUSH_TO_STACK(pulStack, 0x08);
        PUSH_TO_STACK(pulStack, 0x07);
        PUSH_TO_STACK(pulStack, 0x06);
        PUSH_TO_STACK(pulStack, 0x05);
        PUSH_TO_STACK(pulStack, 0x04);
        pulStack++;

        pclThread_->m_pwStackTop = pulStack;
    }
    \endcode

    \section KST Kernel Startup

    The same general process applies to starting the kernel on an ARM Cortex-M0 as
    on other platforms. Here, we initialize and start the platform specific timer
    and software-interrupt modules, find the first thread to run, and then jump to
    that first thread.

    Now, to perform that last step, we have two options:

    1) Simulate a return from an exception manually to start the first thread, or..
    2) Use a software interrupt to trigger the first "Context Restore/Return from Interrupt"

    For 1), we basically have to restore the whole stack manually, not relying on
    the CPU to do any of this for us. That's certainly doable, but not all Cortex
    parts support this (other members of the family support privileged modes,
    etc.). That, and the code required to do this is generally more complex due to
    all of the exception-state simulation. So, we will opt for the second option
    instead.

    To implement a software to start our first thread, we will use the SVC
    instruction to generate an exception. From that exception, we can then restore
    the context from our first thread, set the CPU up to use the right “process”
    stack, and return-from-exception back to our first thread. We'll explore the
    code for that later.

    But, before we can call the SVC exception, we're going to do a couple of things.

    First, we're going to reset the default MSP stack pointer to its original
    top-of-stack value. The rationale here is that we no longer care about the data
    on the MSP stack, since calling the SVC instruction triggers a chain of events
    from which we never return. The MSP is also used by all exception-handling, so
    regaining a few words of stack here can be useful. We'll also enable all
    maskable exceptions at this point, since this code results in the kernel being
    started with the CPU executing the RTOS threads, at which point a user would
    expect interrupts to be enabled.

    Note, the default stack pointer location is stored at address 0x00000000 on all
    ARM Cortex M0 parts. That explains the code below.

    \code
    void ThreadPort_StartFirstThread( void )
    {
        asm(
            " ldr r1, [r0] \n" // Reset the MSP to the default base address
            " msr msp, r1 \n"
            " cpsie i \n"      // Enable interrupts
            " svc 0 \n"        // Jump to SVC Call
            );
    }
    \endcode

    \section FTE First Thread Entry

    This handler has the job of taking the first thread object's stack, and
    restoring the default state data in a way that ensures that the thread starts
    executing when returning from the call.

    We also keep in mind that there's an 8-byte offset from the beginning of the
    thread object to the location of the thread stack pointer. This offset is a
    result of the thread object inheriting from the linked-list node class, which
    has 8-bytes of data. This is stored first in the object, before the first
    element of the class, which is the "stack top" pointer.

    The following assembly code shows how the SVC call is implemented in Mark3 for
    the purpose of starting the first thread.

    \code
    get_thread_stack:
        ; Get the stack pointer for the current thread
        ldr r0, g_pstCurrent
        ldr r1, [r0]
        add r1, #8
        ldr r2, [r1]         ; r2 contains the current stack-top

    load_manually_placed_context_r11_r8:
        ; Handle the bottom 32-bytes of the stack frame
        ; Start with r11-r8, because only r0-r7 can be used
        ; with ldmia on CM0.
        add r2, #16
        ldmia r2!, {r4-r7}
        mov r11, r7
        mov r10, r6
        mov r9, r5
        mov r8, r4

    set_psp:
        ; Since r2 is coincidentally back to where the stack pointer should be,
        ; Set the program stack pointer such that returning from the exception handler
        msr psp, r2

    load_manually_placed_context_r7_r4:
        ; Get back to the bottom of the manually stacked registers and pop.
        sub r2, #32
        ldmia r2!, {r4-r7}  ; Register r4-r11 are restored.

    set_thread_and_privilege_modes:
        ; Also modify the control register to force use of thread mode as well
        ; For CM3 forward-compatibility, also set user mode.
        mrs r0, control
        mov r1, #0x03
        orr r0, r1
        control, r0

    set_lr:
        ; Set up the link register such that on return, the code operates in thread mode using the PSP
        ; To do this, we or 0x0D to the value stored in the lr by the exception hardware EXC_RETURN.
        ; Alternately, we could just force lr to be 0xFFFFFFFD (we know that's what we want from the hardware, anyway)
        mov  r0, #0x0D
        mov  r1, lr
        orr r0, r1

    exit_exception:
        ; Return from the exception handler.  The CPU will automagically unstack R0-R3, R12, PC, LR, and xPSR
        ; for us.  If all goes well, our thread will start execution at the entrypoint, with the us-specified
        ; argument.
        bx r0

    \endcode

    \section CSW Context Switching

    On ARM Cortex parts, there's dedicated hardware that's used primarily to
    support RTOS (or RTOS-like) funcationlity. This functionality includes the
    SysTick timer, and the PendSV Exception. SysTick is used for a tick-based
    kernel timer, while the PendSV exception is used for performing context
    switches. In reality, it's a "special SVC" call that's designed to be
    lower-overhead, in that it isn't mux'd with a bunch of other system or
    application functionality.

    So how do we go about actually implementing a context switch here? There are a
    lot of different parts involved, but it essentially comes down to 3 steps:

    1) Saving the context.  Thread's top-of-stack value is stored, all registers are stacked.  We're good to go!

    2) Swap threads.  We swap the Scheduler's “next” thread with the “current” thread.

    3)  Restore Context.  This is more or less identical to what we did when restoring the first context.  Some operations may be optimized for data already stored in registers.

    The code used to implement these steps on Cortex-M0 is presented below:

    \code
    void PendSV_Handler(void)
    {
        ASM(
        // Thread_SaveContext()
        " ldr r1, CURR_ \n"
        " ldr r1, [r1] \n "
        " mov r3, r1 \n "
        " add r3, #8 \n "

        //  Grab the psp and adjust it by 32 based on the extra registers we're going
        // to be manually stacking.
        " mrs r2, psp \n "
        " sub r2, #32 \n "

        // While we're here, store the new top-of-stack value
        " str r2, [r3] \n "

        // And, while r2 is at the bottom of the stack frame, stack r7-r4
        " stmia r2!, {r4-r7} \n "

        // Stack r11-r8
        " mov r7, r11 \n "
        " mov r6, r10 \n "
        " mov r5, r9 \n "
        " mov r4, r8 \n "
        " stmia r2!, {r4-r7} \n "

        // Equivalent of Thread_Swap() - performs g_pstCurrent = g_pstNext
        " ldr r1, CURR_ \n"
        " ldr r0, NEXT_ \n"
        " ldr r0, [r0] \n"
        " str r0, [r1] \n"

        // Thread_RestoreContext()
        // Get the pointer to the next thread's stack
        " add r0, #8 \n "
        " ldr r2, [r0] \n "

        // Stack pointer is in r2, start loading registers from the "manually-stacked" set
        // Start with r11-r8, since these can't be accessed directly.
        " add r2, #16 \n "
        " ldmia r2!, {r4-r7} \n "
        " mov r11, r7 \n "
        " mov r10, r6 \n "
        " mov r9, r5 \n "
        " mov r8, r4 \n "

        // After subbing R2 #16 manually, and #16 through ldmia, our PSP is where it
        // needs to be when we return from the exception handler
        " msr psp, r2 \n "

        // Pop manually-stacked R4-R7
        " sub r2, #32 \n "
        " ldmia r2!, {r4-r7} \n "

        // lr contains the proper EXC_RETURN value
        // we're done with the exception, so return back to the newly-chosen thread
        " bx lr \n "
        " nop \n "

        // Must be 4-byte aligned.  Also - GNU assembler, I hate you for making me resort to this.
        " NEXT_: .word g_pstNext \n"
        " CURR_: .word g_pstCurrent \n"
        );
    }
    \endcode

    \section KTI Kernel Timers

    ARM Cortex-M series microcontrollers each contain a SysTick timer, which was
    designed to facilitate a fixed-interval RTOS timer-tick. This timer is a
    precise 24-bit down-count timer, run at the main CPU clock frequency, that can
    be programmed to trigger an exception when the timer expires. The handler for
    this exception can thus be used to drive software timers throughout the system
    on a fixed interval.

    Unfortunately, this hardware is extremely simple, and does not offer the
    flexibility of other timer hardware commonly implemented by MCU vendors -
    specifically a suitable timer prescalar that can be used to generate efficient,
    long-counting intervals. As a result, while the "generic" port of Mark3 for
    Cortex-M0 leverages the common SysTick timer interface, it only supports the
    tick-based version of the kernel's timer (note that specific Cortex-M0 ports
    such as the Atmel SAMD20 do have tickless timers).

    Setting up a tick-based KernelTimer class to use the SysTick timer is, however,
    extremely easy, as is illustrated below:

    \code
    void KernelTimer::Start(void)
    {
        SysTick_Config(SYSTEM_FREQ / 1000); // 1KHz fixed clock...
        NVIC_EnableIRQ(SysTick_IRQn);
    }
    \endcode

    In this instance, the call to SysTick_Config() generates a 1kHz system-tick
    signal, and the NVIC_EnableIRQ() call ensures that a SysTick exception is
    generated for each tick. All other functions in the Cortex version of the
    KernelTimer class are essentially stubbed out (see the source for more
    details).

    Note that the functions used in this call are part of the ARM Cortex
    Microcontroller Software Interface Standard (cmsis), and are supplied by all
    parts vendors selling Cortex hardware. This greatly simplifies the design of
    our port-code, since we can be reasonably assured that these APIs will work the
    same on all devices.

    The handler code called when a SysTick exception occurs is basically the same
    as on other platforms (such as AVR), except that we explicitly clear the
    "exception pending" bit before returning. This is implemented in the following
    code:

    \code
    void SysTick_Handler(void)
    {
    #if KERNEL_USE_TIMERS
        TimerScheduler::Process();
    #endif
    #if KERNEL_USE_QUANTUM
        Quantum::UpdateTimer();
    #endif

        // Clear the systick interrupt pending bit.
        SCB->ICSR |= SCB_ICSR_PENDSTCLR_Msk;
    }
    \endcode

    \section CRS Critical Sections

    A "critical section" is a block of code whose execution cannot be interrupted
    by means of context switches or an interrupt. In a traditional single-core
    operating system, it is typically implemented as a block of code where the
    interrupts are disabled - this is also the approach taken by Mark3. Given that
    every CPU has its own means of disabling/enabling interrupts, the
    implementation of the critical section APIs is also non-portable.

    In the Cortex-M0 port, we implement the two critical section APIs (CS_ENTER()
    and CS_EXIT()) as function-like macros containing inline assembly. All uses of
    these calls are called in pairs within a function and must take place at the
    same level-of-scope. Also, as nesting may occur (critical section within a
    critical section), this must be taken into account in the code.

    In general, CS_ENTER() performs the following tasks:
    - Cache the current interrupt-enabled state within a local variable in the thread's state
    - Disable interrupts
    .

    Conversely, CS_EXIT() performs the following tasks:
    - Read the original interrupt-enabled state from the cached value
    - Restore interrupts to the original value
    .

    On Cortex-M series micrcontrollers, the PRIMASK special register contains a
    single status bit which can be used to enable/disable all maskable interrupts
    at once. This register can be read directly to examine or modify its state. For
    convenience, ARMv6m provides two instructions to enable/disable interrupts -
    cpsid (disable interrupts) and cpsie (enable interrupts). Mark3 Implements
    these steps according to the following code:

    \code
    //------------------------------------------------------------------------
    //! Enter critical section (copy current PRIMASK register value, disable interrupts)
    #define CS_ENTER()    \
    {    \
        K_ULONG __ulRegState;    \
        asm    ( \
        " mrs r0, PRIMASK \n"    \
        " mov %[STATUS], r0 \n" \
        " cpsid i \n "    \
        : [STATUS] "=r" (__ulRegState) \
        );

    //------------------------------------------------------------------------
    //! Exit critical section (restore previous PRIMASK status register value)
    #define CS_EXIT() \
        asm    ( \
        " mov r0, %[STATUS] \n" \
        " msr primask, r0 \n"    \
        : \
        : [STATUS] "r" (__ulRegState) \
        ); \
    }
    \endcode

    \section CON Conclusion

    In this chapter we have investigated how the main non-portable areas of the
    Mark3 RTOS are implemented on a Cortex-M0 microcontroller. Mark3 leverages all
    of the hardware blocks designed to enable RTOS functionality on ARM Cortex-M
    series microcontrollers: the SVC call provides the mechanism by which we start
    the kernel, the PendSV exception provides the necessary software interrupt, and
    the SysTick timer provides an RTOS tick. As a result, Mark3 is a perfect fit
    for these devices - and as a result of this approach, the same RTOS port code
    should work with little to no modification on all ARM Cortex-M parts.

    We have discussed what functionality in the RTOS is not portable, and what
    interfaces must be implemented in order to complete a fully-functional port.
    The five specific areas which are non-portable (stack initialization, kernel
    startup/entry, kernel timers, context switching, and critical sections) have
    been discussed in detail, with the platform-specifc source provided as a
    practical reference to ARM-specific OS features, as well as Mark3's porting
    infrastructure. From this example (and the accompanying source), it should be
    possible for an experienced developers to create a port Mark3 to other
    microcontroller targets.
 */
/*!
	\page BUILD0 Build System
	
	Mark3 is distributed with a recursive makefile build system, allowing 
	the entire source tree to be built into a series of libraries with
	simple make commands. The way the scripts work, every directory with a 
	valid makefile is scanned, as well as all of its subdirectories. The 
	build then generates binary components for all of the components it 
	finds -libraries and executables. All libraries that are generated can 
	then be imported into an application using the linker without having to 
	copy-and-paste files on a module-by-module basis. Applications built 
	during this process can then be loaded onto a device directly, without 
	requiring a GUI-based IDE. As a result, Mark3 integrates well with 3rd 
	party tools for continuous-integration and automated testing. 

	This modular framework allows for large volumes of libraries and binaries
	to be built at once - the default build script leverages this to build all 
	of the examples and unit tests at once,	   linking against the pre-built 
	kernel, services, and drivers.	Whatever can be built as a library is 
	built as a library, promoting reuse throughout the platform, and enabling 
	Mark3 to be used as a platform, with an ecosystem of libraries, services,
	drivers and applications. 
	
	\section BUILDLAYOUT Source Layout
	
	One key aspect of Mark3 is that system features are organized into 
	their own separate modules. These modules are further grouped 
	together into folders based on the type of features represented: 

	\verbatim
	Root		 Base folder, contains recursive makefiles for build system
		bootloader	 Mark3 Bootloader code for AVR
		build		 Makefile support for various platforms
		doc			 Documentation (including this)
		drivers		 Device driver code
		example		 Example applications					 
		kernel		 Basic Mark3 Components (the focus of this manual)			  
			cpu		 CPU-specific porting code
		services	 Utility code and services, extended system features
		stage		 Staging directory, where the build system places artifacts
		tests		 Unit tests, written as C/C++ applications
	\endverbatim
	
	\section BUILDKERNEL Building the kernel
	
	The base.mak file determines how the kernel, drivers, and libraries are
	built, for what targets, and with what options.	 Most of these options can 
	be copied directly from the options found in your IDE managed projects. 
	Below is an overview of the main variables used to configure the build.

	\verbatim
	STAGE		  - Location in the filesystem where the build output is stored
	ROOT_DIR	 - The location of the root source tree
	ARCH		- The CPU architecture to build against
	VARIANT		   - The variant of the above CPU to target
	TOOLCHAIN	 - Which toolchain to build with (dependent on ARCH and VARIANT)
	\endverbatim
	
	Build.mak contains the logic which is used to perform the recursive make 
	in all directories. Unless you really know what you're doing, it's best 
	to leave this as-is. 

	You must make sure that all required paths are set in your system 
	environment variables so that they are accessible through from the 
	command-line. 

	Once configured, you can build the source tree using the various make targets:

	- make headers 
	  - copy all headers in each module's /public subdirectory to the location specified by STAGE environment variable's ./inc subdirectory.
	  .
	- make library 
	  - regenerate all objects copy marked as libraries (i.e. the kernel + drivers).  Resulting binaries are copied into STAGE's ./lib subdirectory.
	  .
	- make binary 
	  - build all executable projects in the root directory structure.	In the default distribution, this includes the basic set of demos.
	  .
	.
	
	To add new components to the recursive build system, simply add your 
	code into a new folder beneath the root install location. 

	Source files, the module makefile and private header files go directly 
	in the new folder, while public headers are placed in a ./public
	subdirectory. Create a ./obj directory to hold the output from the 
	builds. 

	The contents of the module makefile looks something like this:

	\code 
		
	# Include common prelude make file
	include $(ROOT_DIR)base.mak

	# If we're building a library, set IS_LIB and LIBNAME
	# If we're building an app, set IS_APP and APPNAME
	IS_LIB=1
	LIBNAME=mylib

	#this is the list of the source modules required to build the kernel
	CPP_SOURCE = mylib.cpp \
				 someotherfile.cpp

	# Similarly, C-language source would be under the C_SOURCE variable.
				 
	# Include the rest of the script that is actually used for building the 
	# outputs
	include $(ROOT_DIR)build.mak
	
	\endcode 
	
	Once you've placed your code files in the right place, and configured 
	the makefile appropriately, a fresh call to make headers, make library, 
	then make binary will guarantee that your code is built. 

	Now, you can still copy-and-paste the required kernel, port, and 
	drivers, directly into your application avoiding the whole process of 
	using make from the command line. To do this, run "make source" from 
	the root directory in svn, and copy the contents of /stage/src into
	your project.  This should contain the source to the kernel, all
	drivers, and all services that are in the tree - along with the necessary
	header files.

	\section WINBUILD Building on Windows

	Building Mark3 on Windows is the same as on Linux, but there are a few
	prerequisites that need to be taken into consideration before the
	build scripts and makefiles will work as expected.

	Step 1 - Install Latest Atmel Studio IDE

	Atmel Studio contains the AVR8 GCC toolchain, which contains the necessary
	compilers, assemblers, and platform support required to turn the source
	modules into libraries and executables.

	To get Atmel Studio, go to the Atmel website (http://www.atmel.com) and
	register to download the latest version.  This is a free download (and
	rather large).	The included IDE (if you choose to use it) is very slick,
	as it's based on Visual Studio, and contains a wonderful cycle-accurate
	simulator for AVR devices.	In fact, the simulator is so good that most
	of the kernel and its drivers were developed using this tool.

	Once you have downloaded and installed Atmel Studio, you will need to
	add the location of the AVR toolcahin to the PATH environment variable.

	To do this, go to Control Panel -> System and Security -> System ->
	Advanced System Settings, and edit the PATH variable.  Append the location
	of the toolchain bin folder to the end of the variable.

	On Windows 7 x64, it should look something like this:

	C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.2.1002\avr8-gnu-toolchain\bin

	Step 2 - Install MinGW and MinSys

	MinGW (and MinSys in particular) provide a unix-like environment that
	runs under windows.	 Some of the utilities provided include a version of
	the bash shell, and GNU standard make - both which are required by
	the Mark3 recursive build system.

	The MinGW installer can be downloaded from its project page on SourceForge.
	When installing, be sure to select the "MinSys" component.

	Once installed, add the MinSys binary path to the PATH environment variable,
	in a similar fashion as with Atmel Studio in Step 1.

	Step 3 - Setup Include Paths in Platform Makefile

	The AVR header file path must be added to the "platform.mak" makefile for
	each AVR Target you are attempting to build for.  These files can be located
	under /embedded/build/avr/atmegaXXX/.  The path to the includes directory
	should be added to the end of the CFLAGS and CPPFLAGS variables, as shown
	in the following:

	\code

	TEST_INC="/c/Program Files (x86)/Atmel/Atmel Toolchain/AVR8 GCC/Native/3.4.2.1002/avr8-gnu-toolchain/include"
	CFLAGS += -I$(TEST_INC)
	CPPFLAGS += -I$(TEST_INC)

	\endcode

	Step 4 - Build Mark3 using Bash

	Launch a terminal to your Mark3 base directory, and cd into the "embedded" folder.
	You should now be able to build Mark3 by running "bash ./build.sh" from the
	command-line.

	Alternately, you can run bash itself, building Mark3 by running ./build.sh or the
	various make targets using the same synatx as documented previously.
		
	Note - building on Windows is *slow*.  This has a lot to do with how "make" performs
	under windows.	There are faster substitutes for make (such as cs-make) that
	are exponentially quicker, and approach the performance of make on Linux.
	Other mechanisms, such as running make with multiple concurrent jobs (i.e. "make -j4")
	also helps significantly, especially on systems with multicore CPUs.

*/
/*!
	\page LICENSE License
	
	\section LIC0 License
	
	Copyright (c) 2013, Funkenstein Software Consulting
	All rights reserved.
	
	Redistribution and use in source and binary forms, with or without
	modification, are permitted provided that the following conditions are met:
		* Redistributions of source code must retain the above copyright
		  notice, this list of conditions and the following disclaimer.
		* Redistributions in binary form must reproduce the above copyright
		  notice, this list of conditions and the following disclaimer in the
		  documentation and/or other materials provided with the distribution.
		* Neither the name of Funkenstein Software Consulting, nor the
		  names of its contributors may be used to endorse or promote products
		  derived from this software without specific prior written permission.
	
	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
	ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
	WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
	DISCLAIMED. IN NO EVENT SHALL FUNKENSTEIN SOFTWARE (MARK SLEVINSKY) BE LIABLE FOR ANY
	DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
	(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
	LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
	ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
	(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
	SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
