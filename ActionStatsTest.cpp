/*
* ActionStats - test program for the "J" interview assignment.
*
* Project default is x64/debug build, to add in interactive debugging.
*
* To execute, run <project directory>\x64\Debug\Action.exe from the
* command line (or the Release subdirectory for release builds).
*
* Action.exe exits with EXIT_SUCCESS (0) or EXIT_FAILURE (1),
* indicating the test results. Test progress is written to
* stdout after each test completes.
*/

#include <iostream>
#include <thread>
#include <sstream>
#include <vector>
#include <mutex>

#include "json.hpp"

#include "ActionStats.h"

using namespace interview;

/*
* OPTIONAL TODO:
*
* In order to avoid complicating the interview solution setup
* by adding an external framework, use the simple macro RUN_TEST.
*/
#define RUN_TEST(testName, testFunction)\
std::cout << testName << ": ";\
if (testFunction())\
{\
    tests::testsPassed++;\
    std::cout << "Success" << std::endl;\
}\
else\
{\
    tests::testsFailed++;\
    std::cout << "Failure" << std::endl;\
}\

namespace tests
{
    std::mutex outputMutex;
    struct ThreadAddAction
    {
        void add(CActionStats& stats, const std::string& action)
        {
            const auto result = stats.AddAction(action);
            if (result != CActionStats::EError::kSuccess)
            {
                std::lock_guard<std::mutex> lock(outputMutex);
                std::cerr << "Error adding action, error="
                    << static_cast<int>(result)
                    << ", action=\"" << action
                    << std::endl;
            }
        }
    };

    const auto sampleAdd = nlohmann::json::array(
        {
            { { "action", "jump" }, { "time", 100.0 } },
            { { "action", "run" }, { "time", 75.0 } },
            { { "action", "jump" }, { "time", 200.0 } }
        });

    const auto sampleResult = nlohmann::json::array(
        {
            { { "action", "jump" }, { "avg", 150.0 } },
            { { "action", "run" }, { "avg", 75.0 } },
        });

    const auto sampleEmptyResult = nlohmann::json::array({});

    auto testsPassed = 0;
    auto testsFailed = 0;

    bool Test1_SampleDataMatch()
    {
        CActionStats actionStats;
        for (const auto& add : sampleAdd)
        {
            actionStats.AddAction(add.dump());
        }

        return actionStats.GetStats() == sampleResult.dump();
    }

    bool Test2_InvalidActionFormat()
    {
        CActionStats actionStats;
        actionStats.AddAction("GARBAGE");

        return sampleEmptyResult.dump() == actionStats.GetStats();
    }

    bool Test3_SampleDataMatchThreaded()
    {
        CActionStats actionStats;
        auto addThread = ThreadAddAction();
        auto testThreads = std::vector<std::thread>(600);
        for (auto i = 0u; i < testThreads.size(); i++)
        {
            const auto& add = sampleAdd[i % 3];
            testThreads[i] = std::thread(&ThreadAddAction::add,
                std::ref(addThread),
                std::ref(actionStats),
                add.dump());
        }

        for (auto& thread : testThreads)
        {
            if (thread.joinable())
            {
                thread.join();
            }
        }

        /*
        * If the test gets this far, it passes. However, to truly
        * detect deadlock, the external test framework has to impose
        * a short run-time limit on this test.
        */
        return true;
    }
}

int main()
{
    /*
    * TODO
    *
    * While I am certain that the action statistics library will not
    * deadlock as it is currently implemented, future modifications
    * might accidentally break concurrency handling and result in
    * deadlock.
    *
    * If the external test framework automation has the ability to
    * kill a process that runs for "too long," a 5 second run-limit on
    * this program should be more than enough time.
    */
    RUN_TEST("Test1_SampleDataMatch", tests::Test1_SampleDataMatch);
    RUN_TEST("Test2_InvalidActionFormat", tests::Test2_InvalidActionFormat);
    RUN_TEST("Test3_SampleDataMatchThreaded", tests::Test3_SampleDataMatchThreaded);

    std::cout << "Passing tests: " << tests::testsPassed << std::endl;
    std::cout << "Failing tests: " << tests::testsFailed << std::endl;

    return tests::testsFailed ? EXIT_FAILURE : EXIT_SUCCESS;
}
