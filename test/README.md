# Communication Library Tests

Contract tests for the Communication Library using Unity framework.

## Structure

```
test/
├── contracts/                # Contract tests (reusable)
│   └── RestInterfaceContractTests.cpp
├── main.cpp                  # Unity entry point
└── CMakeLists.txt           # Build configuration
```

## Running Tests

```bash
mkdir build && cd build
cmake .. -DBUILD_TESTS=ON
cmake --build . --target contract_tests
./test/contract_tests
```

## Adding Tests

1. Create test file in `contracts/`
2. Use `#include <unity.h>`
3. Define tests with `TEST_CASE("test_name")`
4. Add to `CMakeLists.txt`

## Usage

Contract tests verify interface behavior and can be reused across concrete implementations. When implementing concrete classes, inherit from the interface and run the same tests to prove contract compliance.
