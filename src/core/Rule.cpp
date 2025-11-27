#include "Rule.h"


bool Rule::canConnect(const PlayerID a, const PlayerID b) {
      if (a == b) {
            return true;
        }
        return false;
    
}