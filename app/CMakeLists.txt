add_executable(example app.cpp)
set_target_properties(example PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS OFF
)
target_include_directories(example PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)
target_compile_definitions(example PRIVATE
    $<$<BOOL:${LOGGER_SRC_CODE_INFO}>:SRC_CODE_INFO>
    $<$<STREQUAL:${LOGGER_LOG_LEVEL},TRACE>:ENABLE_LOG_LEVEL=LOG_LEVEL_TRACE>
    $<$<STREQUAL:${LOGGER_LOG_LEVEL},DEBUG>:ENABLE_LOG_LEVEL=LOG_LEVEL_DEBUG>
    $<$<STREQUAL:${LOGGER_LOG_LEVEL},INFO>:ENABLE_LOG_LEVEL=LOG_LEVEL_INFO>
    $<$<STREQUAL:${LOGGER_LOG_LEVEL},WARN>:ENABLE_LOG_LEVEL=LOG_LEVEL_WARN>
    $<$<STREQUAL:${LOGGER_LOG_LEVEL},ERROR>:ENABLE_LOG_LEVEL=LOG_LEVEL_ERROR>
    $<$<STREQUAL:${LOGGER_LOG_LEVEL},FATAL>:ENABLE_LOG_LEVEL=LOG_LEVEL_FATAL>
)