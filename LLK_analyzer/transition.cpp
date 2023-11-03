//
// Created by mikhail.syvachenko on 28.10.2023.
//

#include "transition.h"

#include <utility>

transition::transition(char from, vector<char> to) {
    this->from = from;
    this->to = std::move(to);
}

transition::~transition() = default;

char transition::getFrom() const {
    return from;
}

vector<char> transition::getTo() const {
    return to;
}

bool transition::operator==(const transition& other) const
{
    return (other.from == from) && (other.to == to);
}

bool transition::operator!=(const transition& other) const
{
    return !(*this == other);;
}
