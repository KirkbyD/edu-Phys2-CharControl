#include "cCollisionListener.hpp"
#include "cComplexObject.hpp"
#include "cPhysicsManager.hpp"
#include <iostream>

extern float PotionTimer;
extern long PlayerHealth;	//Todo: move to specialized entity object!

void cCollisionListener::Collide(nPhysics::iPhysicsComponent* compA, nPhysics::iPhysicsComponent* compB) {
	//Resolve all collisions here!

	//Only printing out the name of ball type objects that are colliding.
	/*if (compA->GetComponentType() == nPhysics::eComponentType::ball
		&& compB->GetComponentType() == nPhysics::eComponentType::ball)
		std::cout << "Collision:\t"
			<< reinterpret_cast<cComplexObject*>(compA->GetParentPointer())->friendlyName
			<< " on "
			<< reinterpret_cast<cComplexObject*>(compB->GetParentPointer())->friendlyName
			<< std::endl;*/

	if (compA->GetComponentType() == nPhysics::eComponentType::characterController) {
		CharacterCollision(compA, compB);
	}

	else if (compB->GetComponentType() == nPhysics::eComponentType::characterController) {
		CharacterCollision(compB, compA);
	}
}

void cCollisionListener::CharacterCollision(nPhysics::iPhysicsComponent* character, nPhysics::iPhysicsComponent* other) {
	cComplexObject* charEntity = character->GetParentPointer();

	//If we're dying, don't do any collisions!
	if (charEntity->getCurrentAnimationName() == "Death")
		return;

	cComplexObject* otherEntity = other->GetParentPointer();
	if (other->GetComponentType() == nPhysics::eComponentType::plane) {
		//Acting as a respawn mechanic. Should change location to a stored value
		//on a specialized entity type for controllable characters.
		//Expand later to trigger a specific animaton or play specific sound?
		if (other->GetParentPointer()->friendlyName == "Death Plane") {
			std::cout << charEntity->friendlyName
					  << " has hit the Death Plane!" << std::endl;

			// Trigger fall death sfx
			// 3D sound at player's position and velocity at time of collision
			pAudioManager->PlaySfx("Screm", charEntity->getPosition(), charEntity->getVelocity());

			// Play death animation
			charEntity->QueueAnimation("Death");
		}
	}

	else if (other->GetComponentType() == nPhysics::eComponentType::box) {
		//Use capsules to store potions and item drops
		if (other->GetParentPointer()->friendlyName == "Health Potion") {
			std::cout << charEntity->friendlyName
					  << " has picked up a Health Potion!" << std::endl;
			//Play sound
			pAudioManager->PlaySfx("Heal", otherEntity->getPosition(), glm::vec3(0.f));

			// Originally Teleported Health potion away - threw fmod errors.
			// Would likely work better if potion were a ghost object.
			//todo: make generic items for spawning in so we can destroy and create as needed.
			//todo: make a tracker for said spawning
			//otherEntity->SetPosition(glm::vec3(10000.f));

			//Removes the non characer collision body from the world
			pPhysicsManager->WorldRemoveRigidBody(other);
			//Hide graphics component.
			auto models = otherEntity->GetModels();
			for (auto model : models) {
				model->isVisible = false;
			}

			// Todo: heal player
			PlayerHealth += 10000;
			if (PlayerHealth > 25000)
				PlayerHealth = 25000;

			// Todo: start potion timer
			PotionTimer = 0.f;
		}
	}

	else if (otherEntity->friendlyName == "Bonfire") {
		//Don't touch the bonfire, it hurts!
		PlayerHealth -= 100; //per frame!
		if (PlayerHealth <= 0) {
			// Todo - move this to main loop game logic
			// OR make a player entity TakeDamage(long) function!
			charEntity->QueueAnimation("Death");
		}
	}
}
