####
# F Prime CMakeLists.txt:
#
####

register_fprime_library(
    AUTOCODER_INPUTS
        "${CMAKE_CURRENT_LIST_DIR}/HiEchoComponent.fpp"
    SOURCES
        "${CMAKE_CURRENT_LIST_DIR}/HiEchoComponent.cpp"
    DEPENDS
        HiNamespace_Ports
)
