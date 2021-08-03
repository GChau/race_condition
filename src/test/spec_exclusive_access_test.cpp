#include "stdafx.h"
#include "resource_access.h"

TEST_CASE("spec exclusive access", "[spec_exclusive_access]") {

    SECTION("fails to sum value with 2 threads correctly after elapsed time", "[spec_exclusive_access]") {
        const size_t iterations = 500000;
        const int expected_sum = iterations * 2;

        std::chrono::time_point<std::chrono::steady_clock> start = std::chrono::steady_clock::now();
        const std::chrono::seconds max_time(10);

        // Loop for 10 seconds to trigger race condition
        while ((std::chrono::steady_clock::now() - start) < max_time) {
            // Arrange
            int resource = 0;
            resource_access::spec_exclusive_access<int> exclusive_access;

            // Critical code / thread set up
            auto critical_code = [](int& resource_in) { resource_in += 1; };

            // Depending of input thread id, iterate N times and increment resource
            auto test_job = [&](const int thread_id) {
                for (size_t i = 0; i < iterations; ++i) {
                    if (thread_id == 0) {
                        exclusive_access.p0_access(critical_code, std::ref(resource));
                    }
                    else {
                        exclusive_access.p1_access(critical_code, std::ref(resource));
                    }
                }
            };

            // Act
            std::thread p0(test_job, 0);
            std::thread p1(test_job, 1);

            p0.join();
            p1.join();

            // Assert
            if (resource != expected_sum) {
                // The expectation is that  we expect at least one iteration run to not sum to our expected_sum value
                // (due to race conditions)
                SUCCEED();
                return;
            }
        }

        FAIL(("spec_exclusive_access should have failed after " + std::to_string(max_time.count()) + " seconds"));
    }
}
