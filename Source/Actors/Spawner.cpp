//
// Created by Lucas N. Ferreira on 30/09/23.
//

#include "Spawner.h"

#include "../Game.h"
#include "Goomba.h"
#include "Player.h"

Spawner::Spawner(Game* game, float spawnDistance)
    : Actor(game), mSpawnDistance(spawnDistance) {}

void Spawner::OnUpdate(float deltaTime) {
    if (abs(GetGame()->GetPlayer()->GetPosition().x - GetPosition().x) <
        mSpawnDistance) {
        auto goomba = new Goomba(GetGame());
        goomba->SetPosition(GetPosition());
        mState = ActorState::Destroy;
    }
}