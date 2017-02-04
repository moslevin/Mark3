project(kernel)

include("./cpu/${mark3_arch}/${mark3_variant}/${mark3_toolchain}/CMakeLists.txt")

get_property(local_mark3_extra_cxx GLOBAL PROPERTY global_mark3_extra_cxx)
get_property(local_mark3_extra_headers GLOBAL PROPERTY global_mark3_extra_headers)

set(LIB_SOURCES
    atomic.cpp
    autoalloc.cpp
    blocking.cpp
    driver.cpp
    eventflag.cpp
    kernel.cpp
    kernelaware.cpp
    ksemaphore.cpp
    ll.cpp
    mailbox.cpp
    message.cpp
    mutex.cpp
    notify.cpp
    priomap.cpp
    profile.cpp
    quantum.cpp
    scheduler.cpp
    thread.cpp
    threadlist.cpp
    timer.cpp
    timerlist.cpp
    tracebuffer.cpp
    ${local_mark3_extra_cxx}
    )

set(LIB_HEADERS
    public/atomic.h
    public/autoalloc.h
    public/blocking.h
    public/driver.h
    public/eventflag.h
    public/kernelaware.h
    public/ksemaphore.h
    public/ll.h
    public/mailbox.h
    public/mark3.h
    public/mark3cfg.h
    public/message.h
    public/mutex.h
    public/notify.h
    public/priomap.h
    public/profile.h
    public/quantum.h
    public/scheduler.h
    public/thread.h
    public/threadlist.h
    public/timer.h
    public/timerlist.h
    public/tracebuffer.h
    ${local_mark3_extra_cxx}
    )

mark3_add_library(mark3 ${LIB_SOURCES} ${LIB_HEADERS})

target_include_directories(mark3 
    PUBLIC
        public
        cpu/${mark3_arch}/${mark3_variant}/${mark3_toolchain}/public
    )
