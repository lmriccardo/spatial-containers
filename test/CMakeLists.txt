set(UNITTEST_OUTPUT_DIR "${CMAKE_SOURCE_DIR}/bin/test/unittest")
include(GoogleTest)

function(create_gtest_test test_name cpp_file)
    add_executable( ${test_name} ${cpp_file})
    target_link_libraries( ${test_name} PRIVATE ${ARGN} gtest gtest_main )
    target_include_directories( ${test_name} PRIVATE ${CMAKE_SOURCE_DIR}/include )
    set_target_properties( ${test_name} 
        PROPERTIES 
            RUNTIME_OUTPUT_DIRECTORY "${UNITTEST_OUTPUT_DIR}" )
    
    gtest_discover_tests( ${test_name} 
        TEST_PREFIX "${test_name}." 
        WORKING_DIRECTORY ${UNITTEST_OUTPUT_DIR}
        PROPERTIES
            LABELS "unit" 
    )
endfunction()

create_gtest_test(PointTest unittests/point_gtest.cpp)
create_gtest_test(BBoxTest unittests/bbox_gtest.cpp)
create_gtest_test(RTreeTest unittests/rtree_gtest.cpp)