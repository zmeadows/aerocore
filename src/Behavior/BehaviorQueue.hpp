#pragma once

#include "Behavior/Pause.hpp"

#include <variant>

// using BehaviorSpec = std::variant<PauseSpec>;
//
// struct BehaviorQueue {
//    std::vector<std::vector<BehaviorSpec>> queue;
//    u32 idx;
// };
//
// class BehaviorSystem final : public System {
// public:
//     void run(float dt) final {
//         auto CM = get_manager();
//
//         //TODO: return iterator from ComponentManager::remove to allow for iteration with deletion
//         std::vector<UUID> followed_copy;
//
//         for (const UUID uuid : m_followed) {
//             auto& bq = CM->get<BehaviorQueue>(uuid);
//
//             followed_copy.push_back(uuid);
//
//             // 1. remove the old behaviors
//             for (const BehaviorSpec& spec : bq.queue[bq.idx]) {
//                std::visit(
//                    [CM, uuid](auto&& _spec) -> void {
//                        CM->remove<decltype(reify(_spec))>(uuid);
//                    }, spec);
//             }
//
//             bq.idx = (bq.idx + 1) % bq.queue.size();
//
//             // 2. construct/add new behaviors from the next set of specs
//             for (const BehaviorSpec& spec : bq.queue[bq.idx]) {
//                std::visit(
//                    [CM, uuid](auto&& _spec) -> void {
//                        CM->book<decltype(reify(_spec))>(uuid, reify(_spec));
//                    }, spec);
//             }
//         }
//
//         for (const UUID uuid : followed_copy) {
//             CM->remove<BehaviorChangeTag>(uuid);
//         }
//     }
//
//     BehaviorSystem(void) : System("Behavior")
//     {
//         get_manager()->subscribe<BehaviorQueue, BehaviorChangeTag>(this);
//     }
// };
