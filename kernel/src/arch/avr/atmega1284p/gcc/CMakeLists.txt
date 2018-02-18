set(folder_prefix ./cpu/${mark3_arch}/${mark3_variant}/${mark3_toolchain})

set(avr_extra_cxx
    ${folder_prefix}/kernelprofile.cpp
    ${folder_prefix}/kernelswi.cpp
    ${folder_prefix}/kerneltimer.cpp
    ${folder_prefix}/threadport.cpp
    )

set(avr_extra_headers
    ${folder_prefix}/kernelprofile.h
    ${folder_prefix}/kernelswi.h
    ${folder_prefix}/kerneltimer.h
    ${folder_prefix}/portcfg.h
    ${folder_prefix}/threadport.h
    )

set_property(GLOBAL PROPERTY global_mark3_extra_cxx "${avr_extra_cxx}")
set_property(GLOBAL PROPERTY global_mark3_extra_headers "${avr_extra_headers}")
