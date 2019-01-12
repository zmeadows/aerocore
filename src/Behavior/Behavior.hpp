#pragma once

struct BehaviorChangeTag {};

template <class Spec>
typename Spec::Reified reify(const Spec& spec);
