/*
* ActionStats.h - library interface for the "J" interview assignment.
*/

#include "ActionStats.h"

#include <mutex>
#include <sstream>

#include "json.hpp"

namespace interview
{
    /*
    * Note: in order to minimize memory consumption, as well as to
    * avoid statistics calculations involving a large set of actions,
    * I only keep track of unique action types, plus the number of
    * occurrences for each.
    */
    CActionStats::EError CActionStats::AddAction(const std::string& action)
    {
        auto actionType = std::string();
        auto actionTime = 0.0;
        try
        {
            auto json = nlohmann::json().parse(action);
            actionType = json["action"].get<std::string>();
            actionTime = json["time"].get<double>();
        }
        catch (...)
        {
            actionType.clear();
            actionTime = 0;
            return CActionStats::EError::kParseError;
        }

        std::unique_lock<std::shared_mutex> actionLock(m_actionMutex);
        if (!actionLock)
        {
            return EError::kRetryableError;
        }

        auto iter = m_actions.find(actionType);
        if (iter != m_actions.end())
        {
            auto& update = iter->second;
            const auto totalTime = (update.m_average * update.m_occurences) + actionTime;
            update.m_occurences++;
            update.m_average = totalTime / update.m_occurences;
        }
        else
        {
            m_actions[actionType] = { actionTime, 1 };
        }

        /*
        * Note: the lock object *should* unlock the mutex in its destructor,
        * but it does not, risking deadlock.
        *
        * Be sure to keep this in mind if the code is modified to return in
        * more than one place, and explicitly unlock the mutex before returning.
        */
        actionLock.unlock();

        return EError::kSuccess;
    }

    std::string CActionStats::GetStats() const
    {
        constexpr auto emptyStats = "[]";

        std::shared_lock<std::shared_mutex> actionLock(m_actionMutex);
        if (!actionLock)
        {
            return emptyStats;
        }

        auto stats = nlohmann::json::array();
        for (const auto& action : m_actions)
        {
            stats.push_back(
                {
                    {"action", action.first},
                    {"avg", action.second.m_average}
                });
        }

        /*
        * Note: the lock object *should* unlock the mutex in its destructor,
        * but it does not, risking deadlock.
        *
        * Be sure to keep this in mind if the code is modified to return in
        * more than one place, and explicitly unlock the mutex before returning.
        */
        actionLock.unlock();

        return stats.dump();
    }
}
