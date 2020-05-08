//
// Created by Henrik Tseng on 4/25/20.
//

#include "physics/block_contact_listener.h"

#include <Box2D/Dynamics/Contacts/b2Contact.h>
#include <physics/world.h>

namespace tetris {

void BlockContactListener::BeginContact(b2Contact* contact) {
  // check if fixture #1 was in an illegal move
  void* bodyUserData = contact->GetFixtureA()->GetBody()->GetUserData();
  if (bodyUserData)
    static_cast<World*> (bodyUserData)->IllegalMoveCallBack(contact->GetFixtureB()->GetBody());

  // check if fixture #2 was in an illegal move
  bodyUserData = contact->GetFixtureB()->GetBody()->GetUserData();
  if (bodyUserData)
    static_cast<World*>(bodyUserData)->IllegalMoveCallBack(contact->GetFixtureA()->GetBody());
}

void BlockContactListener::EndContact(b2Contact* contact) {

}

}