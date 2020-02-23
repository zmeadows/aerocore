#pragma once

#include "UUID.hpp"
#include "Util.hpp"

#include <string>
#include <future>
#include <vector>

#include "unstd/unstdlib.hpp"
#include "unstd/ArraySet.hpp"
#include "unstd/ThreadPool.hpp"

#define SUBSCRIBE(...) static constexpr auto subscriptions = TypeList<__VA_ARGS__>()

class ComponentManager;
struct TimeDelta;

struct System {
    const std::string name;
    ArraySet<UUID> followed;
    bool active;

    inline bool is_following(const UUID& uuid) const { return followed.contains(uuid); }
    inline void follow(const UUID& uuid) { followed.insert(uuid); }
    inline void unfollow(const UUID& uuid) { followed.remove(uuid); }

    //TODO: make pure virtual
    virtual void run(ComponentManager* CM, TimeDelta dt) = 0;

    System(const std::string& system_name)
        : name(system_name)
        , followed()
        , active(true)
    {}

    System()                         = delete;
    System(const System&)            = delete;
    System& operator=(const System&) = delete;
    System(System&&)                 = delete;
    System& operator=(System&&)      = delete;
};

struct SimpleParallelSystem : System {
    static ThreadPool pool;
    static const size_t nthreads;

    SimpleParallelSystem(const std::string& system_name) : System(system_name) {}

    virtual void pre(ComponentManager* CM) {}
    virtual void par_run(ComponentManager* CM, const Slice<UUID>& entities, TimeDelta dt) = 0;
    virtual void post(ComponentManager* CM) {}

    void run(ComponentManager* CM, TimeDelta dt) final {
        this->pre(CM);

        std::vector<std::future<void>> results;

        for (const auto& slice : this->followed.split(this->nthreads)) {
            results.emplace_back(pool.enqueue([&]() { this->par_run(CM, slice, dt); }));
        }

        for (auto&& x : results) {
            x.get();
        }

        this->post(CM);
    }
};
