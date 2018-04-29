set(folder_prefix ./arch/${mark3_arch}/${mark3_variant}/${mark3_toolchain})

set(arm_extra_cxx
    ${folder_prefix}/kernelprofile.cpp
    ${folder_prefix}/kernelswi.cpp
    ${folder_prefix}/kerneltimer.cpp
    ${folder_prefix}/threadport.cpp
    )

set(arm_extra_headers
    ${folder_prefix}/kernelprofile.h
    ${folder_prefix}/kernelswi.h
    ${folder_prefix}/kerneltimer.h
    ${folder_prefix}/m3_core_cm0.h
    ${folder_prefix}/portcfg.h
    ${folder_prefix}/threadport.h
    )

set(arm_extra_libs
    stm32f429-startup
)

set_property(GLOBAL PROPERTY global_mark3_extra_cxx "${arm_extra_cxx}")
set_property(GLOBAL PROPERTY global_mark3_extra_headers "${arm_extra_headers}")
set_property(GLOBAL PROPERTY global_mark3_extra_libs "${arm_extra_libs}")
