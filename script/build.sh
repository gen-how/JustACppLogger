BUILD_TYPE=${1:-""}
if [[ "$BUILD_TYPE" == "" ]]; then
  echo "No \"BUILD_TYPE\" provided, using default value: \"Release\"."
  BUILD_TYPE="Release"
fi

if [ ! -d "build" ]; then
  mkdir "build"
  echo "No \"build\" dir, auto created."
fi

cd "build"

cmake .. \
  -D CMAKE_EXPORT_COMPILE_COMMANDS=ON \
  -D CMAKE_BUILD_TYPE="${BUILD_TYPE}" \
  -D LOGGER_SRC_CODE_INFO=ON \
  -D LOGGER_LOG_LEVEL=INFO

make
