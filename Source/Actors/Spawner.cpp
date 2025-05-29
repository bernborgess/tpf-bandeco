#include "Spawner.h"

#include "../Game.h"
#include "Goomba.h"
#include "Player.h"

Spawner::Spawner(Game *game, float spawnDistance)
    : Actor(game), mSpawnDistance(spawnDistance) {}

void Spawner::OnUpdate(float deltaTime) {
    auto marioX = mGame->GetMario()->GetPosition().x;
    if (mPosition.x - marioX < mSpawnDistance) {
        auto goomba = new Goomba(mGame);
        goomba->SetPosition(mPosition);
        SetState(ActorState::Destroy);
    }
}
