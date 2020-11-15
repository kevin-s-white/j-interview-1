#pragma once

/*
* ActionStats.h - library interface for the "J" interview assignment.
*/

#include <string>
#include <shared_mutex>
#include <map>

namespace interview
{
    /*
    * CActionStats is a thread-safe class that maintains statistics for
    * different kinds of actions, and returns statistics for the average
    * time of each kind of action.
    *
    * Action names are user-defined.
    * Action times are expected to be non-negative decimal values.
    *
    * See the AddAction() function below and its associated comments
    * describing the JSON input string's contents.
    *
    * See the GetStats() function below and its associated comments
    * describing the JSON output string's contents.
    */
    class CActionStats
    {
    public:
        CActionStats() = default;
        virtual ~CActionStats() = default;

        CActionStats(const CActionStats&) = delete;
        CActionStats& operator=(const CActionStats&) = delete;

        enum class EError
        {
            kSuccess = 0,
            kParseError,
            kRetryableError
        };

        /*
        * Accept a JSON serialized string describing a single add operation.
        *
        * Examples:
        *
        * {"action":"jump", "time": 100}
        * {"action":"run", "time": 75}
        * {"action":"jump", "time": 200}
        */
        EError AddAction(const std::string& action);

        /*
        * Return the average time, as a serialized JSON array, of the
        * successfully added actions.
        *
        * Example return array from the three example adds, assuming they
        * all succeeded:
        *
        * [
        *     {"action":"jump", "avg":150},
        *     {"action":"run", "avg":75}
        * ]
        */
        std::string GetStats() const;

    private:
        struct SStats
        {
            double  m_average;
            size_t  m_occurences;
            SStats(const double average = 0, const size_t occurences = 0)
                : m_average(average), m_occurences(occurences)
            {
            }
        };

        std::map<std::string, SStats>   m_actions;
        mutable std::shared_mutex       m_actionMutex;
    };
}
