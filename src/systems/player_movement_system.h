//
// Created by Kenneth William on 2019-01-31.
//

#ifndef PANDAEXPRESS_MOVEABLE_H
#define PANDAEXPRESS_MOVEABLE_H

#include "system.h"
#include "components/panda.h"

class playerMovementSystem : public System {
public:
    playerMovementSystem();
    virtual void update(Blackboard& blackboard, entt::DefaultRegistry& registry) override;

private:

};


#endif //PANDAEXPRESS_MOVEABLE_H