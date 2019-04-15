//
// Created by cowan on 30/01/19.
//

#ifndef PANDAEXPRESS_PHYSICS_H
#define PANDAEXPRESS_PHYSICS_H

/***
 * The entity assigned the Panda component is able to be controlled by the player_movement_system
 *
 */

struct Panda {
    bool alive;
    bool hurt, invincible, recovering, facingRight;
    bool dead, dying;

    // Indicates that the panda picked up a shield this update
    // Maybe change this into a set of a Powerup enum to handle multiple powerups
    bool gotShield;

    Panda() :
        alive(true),
        hurt(false),
        invincible(false),
        recovering(false),
        facingRight(true),
        dead(false),
        dying(false)
        {}
};

#endif //PANDAEXPRESS_PHYSICS_H