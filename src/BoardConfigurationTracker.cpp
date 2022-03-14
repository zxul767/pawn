#include "BoardConfigurationTracker.hpp"

namespace game_rules
{
bool BoardConfigurationTracker::add_record(const BoardKey &key, ushort &times)
{
    auto iter = tracker.find(key);

    if (iter != tracker.end())
    {
        (*iter).second++;
        times = (*iter).second;

        return false;
    }

    tracker.insert(std::pair<BoardKey, ushort>(key, 1));

    return true;
}

bool BoardConfigurationTracker::decrease_record(const BoardKey &key)
{
    auto iter = tracker.find(key);
    if (iter != tracker.end())
    {
        if ((*iter).second <= 1)
            tracker.erase(iter);
        else
            (*iter).second--;

        return true;
    }

    return false;
}

ushort BoardConfigurationTracker::get_repetitions(const BoardKey &key) const
{
    auto iter = tracker.find(key);

    if (iter != tracker.end())
        return (*iter).second;

    return 0;
}

void BoardConfigurationTracker::reset()
{
    tracker.clear();
}

} // namespace game_rules
