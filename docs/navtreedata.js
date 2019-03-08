var NAVTREE =
[
  [ "Mark3 Realtime Kernel", "index.html", [
    [ "The Mark3 Realtime Kernel", "index.html", null ],
    [ "License", "_l_i_c_e_n_s_e.html", [
      [ "License", "_l_i_c_e_n_s_e.html#LIC0", null ]
    ] ],
    [ "Configuring The Mark3 Kernel", "_c_o_n_f_i_g0.html", [
      [ "Overview", "_c_o_n_f_i_g0.html#CONFIG_I", [
        [ "Kernel Configuration Options", "_c_o_n_f_i_g0.html#CONFIG_MARK3CFG", null ],
        [ "Port Configuration Options", "_c_o_n_f_i_g0.html#CONFIG_PORTCFG", null ]
      ] ]
    ] ],
    [ "Building Mark3", "_b_u_i_l_d0.html", null ],
    [ "Getting Started With The Mark3 API", "_s_t_a_r_t.html", [
      [ "Kernel Setup", "_s_t_a_r_t.html#START0", null ],
      [ "Threads", "_s_t_a_r_t.html#THREADS", [
        [ "Thread Setup", "_s_t_a_r_t.html#TH_SETUP", null ],
        [ "Entry Functions", "_s_t_a_r_t.html#TH_ENTRY", null ]
      ] ],
      [ "Timers", "_s_t_a_r_t.html#TIMERS", null ],
      [ "Semaphores", "_s_t_a_r_t.html#SEM", null ],
      [ "Mutexes", "_s_t_a_r_t.html#MUT", null ],
      [ "Event Flags", "_s_t_a_r_t.html#EVF", null ],
      [ "Messages", "_s_t_a_r_t.html#MSG", [
        [ "Message Objects", "_s_t_a_r_t.html#MSGO", null ],
        [ "Message Queues", "_s_t_a_r_t.html#MSGQ", null ],
        [ "Messaging Example", "_s_t_a_r_t.html#MSGQS", null ]
      ] ],
      [ "Mailboxes", "_s_t_a_r_t.html#MBX", [
        [ "Mailbox Example", "_s_t_a_r_t.html#MBXE", null ]
      ] ],
      [ "Notification Objects", "_s_t_a_r_t.html#NTF", [
        [ "Notification Example", "_s_t_a_r_t.html#NTFE", null ]
      ] ],
      [ "Condition Variables", "_s_t_a_r_t.html#CONDVAR", [
        [ "Condition Variable Example", "_s_t_a_r_t.html#CONDVAREX", null ]
      ] ],
      [ "Reader-Write Locks", "_s_t_a_r_t.html#RWLOCKS", [
        [ "Reader-Write Lock Example", "_s_t_a_r_t.html#RWLOCKEX", null ]
      ] ],
      [ "Sleep", "_s_t_a_r_t.html#SLP", null ],
      [ "Round-Robin Quantum", "_s_t_a_r_t.html#RR", null ]
    ] ],
    [ "Why Mark3?", "_w_h_y_m_a_r_k3.html", null ],
    [ "When should you use an RTOS?", "_w_h_e_n_r_t_o_s.html", [
      [ "The reality of system code", "_w_h_e_n_r_t_o_s.html#WHENREAL", null ],
      [ "Superloops, and their limitations", "_w_h_e_n_r_t_o_s.html#WHENSUPER", [
        [ "Intro to Superloops", "_w_h_e_n_r_t_o_s.html#WHENSUPERINTRO", null ],
        [ "The simplest loop", "_w_h_e_n_r_t_o_s.html#WHENSUPERSIMPLE", null ],
        [ "Interrupt-Driven Super-loop", "_w_h_e_n_r_t_o_s.html#WHENSUPERINTER", null ],
        [ "Cooperative multi-tasking", "_w_h_e_n_r_t_o_s.html#WHENSUPERCOOP", null ],
        [ "Hybrid cooperative/preemptive multi-tasking", "_w_h_e_n_r_t_o_s.html#WHENSUPERHYBRID", null ]
      ] ],
      [ "Problems with superloops", "_w_h_e_n_r_t_o_s.html#WHENPROB", [
        [ "Hidden Costs", "_w_h_e_n_r_t_o_s.html#WHENPROBHIDDEN", null ],
        [ "Tightly-coupled code", "_w_h_e_n_r_t_o_s.html#WHENPROBTIGHTLY", null ],
        [ "No blocking Calls", "_w_h_e_n_r_t_o_s.html#WHENPROBBLOCK", null ],
        [ "Difficult to guarantee responsiveness", "_w_h_e_n_r_t_o_s.html#WHENPROBDIFF", null ],
        [ "Limited preemption capability", "_w_h_e_n_r_t_o_s.html#WHENPROBLIM", null ]
      ] ]
    ] ],
    [ "Can you afford an RTOS?", "_c_a_n_r_t_o_s.html", [
      [ "Intro", "_c_a_n_r_t_o_s.html#CANRTOSINTRO", null ],
      [ "Application description", "_c_a_n_r_t_o_s.html#CANRTOSAPP", null ],
      [ "Runtime Overhead", "_c_a_n_r_t_o_s.html#CANRTOSOVERHEAD", null ],
      [ "Analysis", "_c_a_n_r_t_o_s.html#CANRTOSANALYSIS", null ]
    ] ],
    [ "Mark3 Design Goals", "_i_n_s_i_d_e_m_a_r_k3.html", [
      [ "Overview", "_i_n_s_i_d_e_m_a_r_k3.html#INSIDEOVERVIEW", [
        [ "Services Provided by an RTOS Kernel", "_i_n_s_i_d_e_m_a_r_k3.html#INSIDEOVERSERVICES", null ],
        [ "Guiding Principles of Mark3", "_i_n_s_i_d_e_m_a_r_k3.html#INSIDEOVERPRINCIPLES", null ],
        [ "Be feature competitive", "_i_n_s_i_d_e_m_a_r_k3.html#INSIDEOVERBFC", null ],
        [ "No external dependencies, no new language features", "_i_n_s_i_d_e_m_a_r_k3.html#INSIDEOVERNED", null ],
        [ "Target the most popular hobbyist platforms available", "_i_n_s_i_d_e_m_a_r_k3.html#INSIDEOVERHPA", null ],
        [ "Maximize determinism – but be pragmatic", "_i_n_s_i_d_e_m_a_r_k3.html#INSIDEOVERMAXDET", null ],
        [ "Apply engineering principles – and that means discipline, measurement and verification", "_i_n_s_i_d_e_m_a_r_k3.html#INSIDEOVERENG", null ],
        [ "Use Virtualization For Verification", "_i_n_s_i_d_e_m_a_r_k3.html#INSIDEOVERFLAVR", null ]
      ] ]
    ] ],
    [ "Mark3 Kernel Architecture", "_m_a_r_k3_k_a.html", [
      [ "Overview", "_m_a_r_k3_k_a.html#MARK3KAOVER", null ],
      [ "Threads and Scheduling", "_m_a_r_k3_k_a.html#THREADSCHED", [
        [ "A Bit About Threads", "_m_a_r_k3_k_a.html#BITTHREAD", null ],
        [ "Thread States and ThreadLists", "_m_a_r_k3_k_a.html#THREADLISTS", null ],
        [ "Blocking and Unblocking", "_m_a_r_k3_k_a.html#BLOCKUNBLOCK", null ],
        [ "Blocking Objects", "_m_a_r_k3_k_a.html#BLOCKOBJ", null ]
      ] ],
      [ "Inside the Mark3 Scheduler", "_m_a_r_k3_k_a.html#INSIDESCHED", [
        [ "Considerations for Round-Robin Scheduling", "_m_a_r_k3_k_a.html#RRSCHED", null ],
        [ "Context Switching", "_m_a_r_k3_k_a.html#CONSWITCH", null ],
        [ "Putting It All Together", "_m_a_r_k3_k_a.html#ALLTOGETHER", null ]
      ] ],
      [ "Timers", "_m_a_r_k3_k_a.html#ARCHTIMERS", [
        [ "Tick-based Timers", "_m_a_r_k3_k_a.html#TICKBASEDTIMERS", null ],
        [ "Tickless Timers", "_m_a_r_k3_k_a.html#TICKLESSTIMERS", null ],
        [ "Timer Processing Algorithm", "_m_a_r_k3_k_a.html#TIMERPA", null ]
      ] ],
      [ "Synchronization and IPC", "_m_a_r_k3_k_a.html#SYNCHIPC", null ],
      [ "Blocking Objects", "_m_a_r_k3_k_a.html#BLOCKINGOBJECTS", [
        [ "Semaphores", "_m_a_r_k3_k_a.html#SEMAPHORES", null ],
        [ "Mutex", "_m_a_r_k3_k_a.html#MUTEX", null ],
        [ "Event Flags", "_m_a_r_k3_k_a.html#EVENTFLAG", null ],
        [ "Notification Objects", "_m_a_r_k3_k_a.html#NOTIFICATION", null ]
      ] ],
      [ "Messages and Message Queues", "_m_a_r_k3_k_a.html#MSGMSGQ", [
        [ "Messages", "_m_a_r_k3_k_a.html#MESSAGES", null ],
        [ "Message Objects", "_m_a_r_k3_k_a.html#MESSAGEOBJ", null ],
        [ "Message Queues", "_m_a_r_k3_k_a.html#MESSAGEQUEUES", null ],
        [ "Mailboxes", "_m_a_r_k3_k_a.html#MAILBOXES", null ],
        [ "Atomic Operations", "_m_a_r_k3_k_a.html#ATOMICOPS", null ],
        [ "Drivers", "_m_a_r_k3_k_a.html#DRIVERS", null ]
      ] ],
      [ "Kernel Proper and Porting", "_m_a_r_k3_k_a.html#KRNLPROPER", null ]
    ] ],
    [ "Mark3C - C-language API bindings for the Mark3 Kernel.", "_m_a_r_k3_c.html", [
      [ "API Conventions", "_m_a_r_k3_c.html#MARK3CAPI", null ],
      [ "Allocating Objects", "_m_a_r_k3_c.html#MARK3CALLOC", null ]
    ] ],
    [ "Release Notes", "_r_e_l_e_a_s_e.html", [
      [ "R7 (Full Throttle) Release", "_r_e_l_e_a_s_e.html#RELR7", null ],
      [ "R6 Release", "_r_e_l_e_a_s_e.html#RELR6", null ],
      [ "R5 Release", "_r_e_l_e_a_s_e.html#RELR5", null ],
      [ "R4 Release", "_r_e_l_e_a_s_e.html#RELR4", null ],
      [ "R3 Release", "_r_e_l_e_a_s_e.html#RELR3", null ],
      [ "R2", "_r_e_l_e_a_s_e.html#RELR2", null ],
      [ "R1 - 2nd Release Candidate", "_r_e_l_e_a_s_e.html#RELR12", null ],
      [ "R1 - 1st Release Candidate", "_r_e_l_e_a_s_e.html#RELR1", null ]
    ] ],
    [ "Code Size Profiling", "_s_i_z_e_p_r_o_f_i_l_e.html", [
      [ "Information", "_s_i_z_e_p_r_o_f_i_l_e.html#SIZEPROFILEINFO", null ],
      [ "Compiler Version", "_s_i_z_e_p_r_o_f_i_l_e.html#SIZEPROFILEVER", null ],
      [ "Profiling Results", "_s_i_z_e_p_r_o_f_i_l_e.html#SIZEPROFILERES", null ]
    ] ],
    [ "Namespaces", null, [
      [ "Namespace List", "namespaces.html", "namespaces" ],
      [ "Namespace Members", "namespacemembers.html", [
        [ "All", "namespacemembers.html", null ],
        [ "Functions", "namespacemembers_func.html", null ],
        [ "Typedefs", "namespacemembers_type.html", null ],
        [ "Enumerations", "namespacemembers_enum.html", null ]
      ] ]
    ] ],
    [ "Classes", "annotated.html", [
      [ "Class List", "annotated.html", "annotated_dup" ],
      [ "Class Index", "classes.html", null ],
      [ "Class Hierarchy", "hierarchy.html", "hierarchy" ],
      [ "Class Members", "functions.html", [
        [ "All", "functions.html", "functions_dup" ],
        [ "Functions", "functions_func.html", "functions_func" ],
        [ "Variables", "functions_vars.html", null ]
      ] ]
    ] ],
    [ "Files", null, [
      [ "File List", "files.html", "files" ],
      [ "File Members", "globals.html", [
        [ "All", "globals.html", null ],
        [ "Functions", "globals_func.html", null ],
        [ "Typedefs", "globals_type.html", null ],
        [ "Enumerations", "globals_enum.html", null ],
        [ "Enumerator", "globals_eval.html", null ],
        [ "Macros", "globals_defs.html", null ]
      ] ]
    ] ],
    [ "Examples", "examples.html", "examples" ]
  ] ]
];

var NAVTREEINDEX =
[
"_b_u_i_l_d0.html",
"class_mark3_1_1_message_pool.html#a0140ae8384d2850f591dfb0482ad1bcd",
"functions_e.html",
"notify_8cpp_source.html"
];

var SYNCONMSG = 'click to disable panel synchronisation';
var SYNCOFFMSG = 'click to enable panel synchronisation';