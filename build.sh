#!/bin/bash
# Build script for LLVM Compiler using Flex and Bison
set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR"

echo "=========================================="
echo "LLVM Compiler Build Script"
echo "Using Flex, Bison, and LLVM"
echo "=========================================="

# Check for required tools
echo ""
echo "[Checking dependencies]..."
for tool in cmake g++ flex bison llvm-config; do
    if command -v "$tool" &> /dev/null; then
        echo "  ✓ $tool found"
    else
        echo "  ✗ $tool NOT found"
        exit 1
    fi
done

# Configure with CMake
echo ""
echo "[Configuring project]..."
mkdir -p build
cd build

cmake .. \
    -DCMAKE_BUILD_TYPE=Debug \
    -DCMAKE_CXX_COMPILER=g++ \
    -DFLEX_OUTPUT_HEADER=${CMAKE_SOURCE_DIR}/src/lex.cpp \
    -DBISON_OUTPUT_HEADER=${CMAKE_BINARY_DIR}/parser.h \
    -G "Unix Makefiles"

# Build the project
echo ""
echo "[Building project]..."
cmake --build . --target all -j$(nproc)

# Show build artifacts
echo ""
echo "[Build complete!]"
echo "=========================================="
echo "Generated files:"
ls -la *.cpp *.h 2>/dev/null || true
echo ""
echo "Executable location: compiler"
echo ""
echo "Usage:"
echo "  ./compiler <input_file.txt>"
echo "  echo 'int main() { return 0; }' | ./compiler"
echo ""

cd ..