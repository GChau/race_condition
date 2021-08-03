#pragma once

// STD / STL includes
#include <atomic>
#include <functional>
#include <type_traits>

namespace resource_access
{
    /*
    * This is the implementation as specified in the Technical Challenge
    * However, this is a ToC ToU race condition between checking flag and setting turn
    * Despite being atomic, p0 may set their flag and enter their critical code after p1 has checked p0's flag
    * and before p1 has set turn to 0. More notes in README.txt.
    */
    template<typename Resource>
    class spec_exclusive_access
    {
        public:
            // Constructor
            spec_exclusive_access(
            ) {
            }

            // Access Methods
            void p0_access(
                const std::function<void(Resource&)>& critical_code,
                Resource& resource
            ) {
                // Raise flag to indicate desire to enter critical code
                flags_[0] = true;
                // If not p0's turn...
                while (turn_ != 0) {
                    while (flags_[1]) {
                        // Spin wait until p1 lowers its flag
                    }
                    // DANGER! ToC ToU race condition here!
                    turn_ = 0;
                }

                // Critical code
                critical_code(resource);

                flags_[0] = false;
            }

            void p1_access(
                const std::function<void(Resource&)>& critical_code,
                Resource& resource
            ) {
                // Raise flag to indicate desire to enter critical code
                flags_[1] = true;
                // If not p1's turn...
                while (turn_ != 1) {
                    while (flags_[0]) {
                        // Spin wait until p0 lowers its flag
                    }
                    // DANGER! ToC ToU race condition here!
                    turn_ = 1;
                }

                // Critical code
                critical_code(resource);

                flags_[1] = false;
            }

        private:
            // flags indicate that a given thread wants to enter its critical section
            std::atomic_bool flags_[2] {false, false};
            // turn indicates which thread had entered its critical section last
            std::atomic_uint turn_{0};
    }; // spec_exclusive_access

    /*
    * This is the fixed implementation, Peterson's Algorithm that works for 2 threads.
    */
    template<typename Resource>
    class fixed_exclusive_access
    {
    public:
        // Constructor
        fixed_exclusive_access(
        ) {
        }

        // Access Methods
        void p0_access(
            const std::function<void(Resource&)>& critical_code,
            Resource& resource
        ) {
            // Raise flag to indicate desire to enter critical code
            flags_[0] = true;
            // Yield priority to p1
            turn_ = 1;
            // Spin wait until p1 has lowered its flag or it is p0's turn
            while (turn_ == 1 && flags_[1]) {
            }

            // Critical code
            critical_code(resource);

            // Lower flag
            flags_[0] = false;
        }

        void p1_access(
            const std::function<void(Resource&)>& critical_code,
            Resource& resource
        ) {
            // Raise flag to indicate desire to enter critical code
            flags_[1] = true;
            // Yield priority to p0
            turn_ = 0;
            // Spin wait until p0 has lowered its flag or it is p1's turn
            while (turn_ == 0 && flags_[0]) {
            }

            // Critical code
            critical_code(resource);

            // Lower flag
            flags_[1] = false;
        }

    private:
        // flags indicate that a given thread wants to enter its critical section
        std::atomic_bool flags_[2] {false, false};
        // turn indicates which thread had entered its critical section last
        std::atomic_uint turn_{ 0 };
    }; // fixed_exclusive_access

} // resource_access