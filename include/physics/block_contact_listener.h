//
// Created by Henrik Tseng on 4/25/20.
//

#ifndef FINALPROJECT_BLOCK_CONTACT_LISTENER_H
#define FINALPROJECT_BLOCK_CONTACT_LISTENER_H

#include <Box2D/Dynamics/b2WorldCallbacks.h>


namespace tetris {

/**
 * Deals with block collision,
 * allowing ability to manual snap blocks into place.
 */
class BlockContactListener : public b2ContactListener {
  void BeginContact(b2Contact* contact);

  void EndContact(b2Contact* contact);
};

} // namespace tetris

#endif  // FINALPROJECT_BLOCK_CONTACT_LISTENER_H
