#include "Spell.h"
#include <algorithm>

namespace RPG {

bool Spell::canBeUsedBy(const std::string& className) const {
    return std::find(classes.begin(), classes.end(), className) != classes.end();
}

} // namespace RPG
