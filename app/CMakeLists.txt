add_executable(example app.cpp)
set_target_properties(example PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS OFF
)
target_include_directories(example PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)
# Link dependancies.
#target_link_libraries(TargetName
#    OtherTargetName
#)