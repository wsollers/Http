#/bin/sh

#cd ../build || exit
cd .. || exit

# Clean previous builds
rm -rf build

# Build with coverage flags
cmake -DCMAKE_CXX_FLAGS="-O0" -DCMAKE_BUILD_TYPE=Debug -B build
cmake --build build

# Run tests
cd build && ctest

# Generate coverage report
lcov --capture --directory . --output-file coverage.info --ignore-errors mismatch,empty
lcov --remove coverage.info '/usr/*' 'gtest/*' --output-file coverage.filtered.info
genhtml coverage.filtered.info --output-directory coverage_html

# Open coverage report
xdg-open coverage_html/index.html

# Clean up
cd ../scripts || exit

