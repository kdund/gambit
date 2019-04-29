
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules")
include(cmake/scan_python_utils.cmake)
include(cmake/boost_python.cmake)
#include(cmake/yaml.cmake)

#find_package(NumPy)

if(PYTHONINTERP_FOUND AND PYTHONLIBS_FOUND)# AND NUMPY_FOUND)

    ################# libscannerbit ##################

    set(libscannerbit_sources 
        ${CMAKE_CURRENT_SOURCE_DIR}/src/diagnostics.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/run_scan.cpp
        $<TARGET_OBJECTS:ScannerBit>
        $<TARGET_OBJECTS:Printers>
        ${GAMBIT_BASIC_COMMON_OBJECTS}
    )

    set(libscannerbit_headers 
        ${CMAKE_CURRENT_SOURCE_DIR}/include/diagnostics.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/python_utils.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/run_scan.hpp
    )

    set(libscannerbit_include_dirs
        ${mkpath_INCLUDE_DIR}
        ${INCLUDE_DIRECTORIES}
        ${MPI_C_INCLUDE_PATH}
        ${MPI_CXX_INCLUDE_PATH}
        ${yaml_INCLUDE_DIR}
        ${GAMBIT_INCDIRS}
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        #${CMAKE_CURRENT_SOURCE_DIR}/contrib/boost_python/include
    )

    set(libscannerbit_libraries
        yaml-cpp
    )
    
    add_library(libscannerbit SHARED ${libscannerbit_sources} ${libscannerbit_headers} )
    
    target_link_libraries(libscannerbit ${libscannerbit_libraries})
    
    set_target_properties(libscannerbit PROPERTIES 
                                        #CXX_VISIBILITY_PRESET default
                                        #VISIBILITY_INLINES_HIDDEN FALSE
                                        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lib"
                                        LIBRARY_OUTPUT_NAME "ScannerBit"
                                        INCLUDE_DIRECTORIES "${libscannerbit_include_dirs}"
                                        LINKER_LANGUAGE CXX
                                        SUFFIX .so)

    add_gambit_deps(libscannerbit)

    ################## scan_python ##################
    
    set(scan_module_libraries
        ${PYTHON_LIBRARIES}
        ${CMAKE_CURRENT_SOURCE_DIR}/contrib/boost_python/lib/libboost_python${PYTHON_VERSION_STRING}.so
        yaml-cpp
    )
    
     set(scan_module_include_dirs
        #${GAMBIT_INCDIRS}
        #${mkpath_INCLUDE_DIR}
        ${yaml_INCLUDE_DIR}
        ${PYTHON_INCLUDE_DIRS}
        ${PROJECT_SOURCE_DIR}/ScannerBit/include
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/contrib/boost_python/include
    )
    
    set(scan_module_headers
        ${CMAKE_CURRENT_SOURCE_DIR}/include/diagnostics.hpp
    )
    
    set(scan_module_sources
        ${CMAKE_CURRENT_SOURCE_DIR}/src/python.cpp
    )
    
    set(scan_module_rpaths
        ${CMAKE_CURRENT_SOURCE_DIR}/lib
        ${CMAKE_CURRENT_SOURCE_DIR}/contrib/boost_python/lib
    )

    
    
    add_library(scan_python SHARED ${scan_module_sources} ${scan_module_headers} )
    
    target_link_libraries(scan_python ${scan_module_libraries})

    set_target_properties(scan_python PROPERTIES 
                                        #CXX_VISIBILITY_PRESET default
                                        #VISIBILITY_INLINES_HIDDEN FALSE
                                        INSTALL_RPATH "${scan_module_rpaths}"
                                        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
                                        LIBRARY_OUTPUT_NAME "ScannerBit"
                                        INCLUDE_DIRECTORIES "${scan_module_include_dirs}"
                                        LINKER_LANGUAGE CXX
                                        PREFIX ""
                                        SUFFIX .so)
                                    
    add_dependencies(scan_python boost_python)
    add_dependencies(scan_python libscannerbit)
    
    #################################################

else(PYTHONINTERP_FOUND AND PYTHONLIBS_FOUND)# AND NUMPY_FOUND)

    message("${BoldRed}-- The ScannerBit python interface requires python, the python libraries, and numpy ${ColourReset}")
    message("${Red}-- PYTHONINTERP_FOUND=${PYTHONINTERP_FOUND} ${ColourReset}")
    message("${Red}-- PYTHONLIBS_FOUND=${PYTHONLIBS_FOUND} ${ColourReset}")
    message("${Red}-- NUMPY_FOUND=${NUMPY_FOUND} ${ColourReset}")

endif(PYTHONINTERP_FOUND AND PYTHONLIBS_FOUND)# AND NUMPY_FOUND)
