//
// Created by Kenneth William on 2019-03-27.
//

#ifndef PANDAEXPRESS_A_STAR_SYSTEM_H
#define PANDAEXPRESS_A_STAR_SYSTEM_H

#include "system.h"
#include "components/panda.h"
#include "components/jacko.h"
#include "components/transform.h"
#include "components/collidable.h"
#include "util/csv_reader.h"
#include "util/constants.h"
#include <iostream>
#include "util/Location.h"

class AStarSystem: public System{
public:
    AStarSystem();
    virtual void update(Blackboard &blackboard, entt::DefaultRegistry &registry);
    void createGrid(Blackboard &blackboard, entt::DefaultRegistry &registry);
    std::vector<Location*> findPath(Location* start, Location* end);
    std::vector<Location*> getProjectilePath(Blackboard &blackboard, entt::DefaultRegistry &registry);
    Location* getGridLocation(float x, float y);
};


#endif //PANDAEXPRESS_A_STAR_SYSTEM_Hs
