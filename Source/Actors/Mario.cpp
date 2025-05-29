//
// Created by Lucas N. Ferreira on 03/08/23.
//

#include "Mario.h"
#include "../Game.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/DrawComponents/DrawAnimatedComponent.h"
#include "../Components/DrawComponents/DrawPolygonComponent.h"
#include "../Components/ColliderComponents/AABBColliderComponent.h"

Mario::Mario(Game *game, const float forwardSpeed, const float jumpSpeed)
    : Actor(game)
      , mForwardSpeed(forwardSpeed)
      , mJumpSpeed(jumpSpeed)
      , mIsRunning(false)
      , mIsDead(false)
      , mRigidBodyComponent(nullptr)
      , mDrawComponent(nullptr)
      , mColliderComponent(nullptr) {

    mRigidBodyComponent = new RigidBodyComponent(this, 1.0f, 5.0);
    // mRigidBodyComponent->SetApplyGravity(false);

    // --------------
    // TODO - PARTE 3
    // --------------

    // TODO 8 (~1 linhas): Crie um componente AABBColliderComponent passando as dimensões
    //  (0,0,Game::TILE_SIZE - 4.0f, Game::TILE_SIZE) e o tipo ColliderLayer::Player da AABB. Se quiser desenhar
    //  a AABB do Mario para testar as colisões, crie um componente `DrawPolygonComponent` com os vértices da AABB.
    //  Armazene o ponteiro desse componente no atributo `mColliderComponent` da classe Mario.
    mColliderComponent = new AABBColliderComponent(this, 0, 0, Game::TILE_SIZE - 4.0f, Game::TILE_SIZE,
                                                   ColliderLayer::Player);
    // new DrawPolygonComponent(this,);


    // --------------
    // TODO - PARTE 4
    // --------------

    // TODO 4.1 (~1 linhas): Crie um componente DrawAnimatedComponent passando os caminhos da imagem (.png) e
    //  dos dados (.json) do sprite sheet do Mário que você criou com o FreeTexturePacker. Armazém o ponteiro
    //  desse componente no atributo `mDrawComponent` da classe Mario.
    mDrawComponent = new DrawAnimatedComponent(this, "../Assets/Sprites/Mario/texture.png",
                                               "../Assets/Sprites/Mario/texture.json");


    // TODO 4.2 (~4 linhas): Utilize a função `AddAnimation` para adicionar as animações "dead", "idle",
    //  "jump" e "run".
    mDrawComponent->AddAnimation("dead", {0});
    mDrawComponent->AddAnimation("idle", {1});
    mDrawComponent->AddAnimation("jump", {2});
    mDrawComponent->AddAnimation("run", {3, 4, 5});

    // TODO 3.4 (~2 linhas): Utilize a função `SetAnimation` para definir a animação inicial como "idle". Em seguida,
    //  utilize a função `SetAnimFPS` para definir a taxa de atualização de quadros da animação para 10.0f.
    mDrawComponent->SetAnimation("idle");
    mDrawComponent->SetAnimFPS(10.0f);
}

void Mario::OnProcessInput(const uint8_t *keyState) {
    if (keyState[SDL_SCANCODE_D] || keyState[SDL_SCANCODE_RIGHT]) {
        mRigidBodyComponent->ApplyForce(mForwardSpeed * Vector2(1, 0));
        mRotation = 0;
        mIsRunning = true;
    } else if (keyState[SDL_SCANCODE_A] || keyState[SDL_SCANCODE_LEFT]) {
        mRigidBodyComponent->ApplyForce(mForwardSpeed * Vector2(-1, 0));
        mRotation = Math::Pi;
        mIsRunning = true;
    } else {
        mIsRunning = false;
    }


    // --------------
    // TODO - PARTE 3
    // --------------

    // TODO 9 (~3 linhas): Verifique se o jogador está no chão (`mIsOnGround`) e se a tecla `space` foi pressionada.
    //  Se sim, aplique uma força vertical com magnitude `mJumpSpeed` e altere a variável `mIsOnGround` para falso.
    if ((keyState[SDL_SCANCODE_SPACE] || keyState[SDL_SCANCODE_W] || keyState[SDL_SCANCODE_UP]) && mIsOnGround) {
        // SDL_Log("BANG! at %f %f", mPosition.x, mPosition.y);
        mRigidBodyComponent->ApplyForce(mJumpSpeed * Vector2::UnitY);
        mIsOnGround = false;
    }
}

void Mario::OnUpdate(float deltaTime) {
    if (mRigidBodyComponent && mRigidBodyComponent->GetVelocity().y != 0) {
        mIsOnGround = false;
    }

    float cameraX = mGame->GetCameraPos().x;
    if (mPosition.x < cameraX) {
        mPosition.x = cameraX;
    }

    // --------------
    // TODO - PARTE 5
    // --------------

    // TODO 7 (~2 linhas): Verifique se a posição vertical do jogador é maior do que o tamanho da tela.
    //  Se for, chame o método `Kill`.
    if (mPosition.y > Game::LEVEL_HEIGHT * Game::TILE_SIZE) {
        Kill();
    }

    ManageAnimations();
}

void Mario::ManageAnimations() {
    // --------------
    // TODO - PARTE 4
    // --------------
    if (mIsDead) return mDrawComponent->SetAnimation("dead");

    // TODO 5.1 (~7-10 linhas): para implementar a troca de animação, basta utilizar os atributos `mIsDead` para
    //  verificar se o jogador está morto, `mIsOnGround` se o jogador está no chão e `mIsRunning` se o jogador
    //  está correndo.
    //  -5.1.1: Se estiver vivo, no chão e correndo, altere a animação para `run`
    if (mIsOnGround && mIsRunning) return mDrawComponent->SetAnimation("run");
    //  -5.1.2: Se estiver vivo, no chão e não estiver correndo, altere a animação para `idle`
    if (mIsOnGround)return mDrawComponent->SetAnimation("idle");
    //  -5.1.3: Se estiver vivo e não estiver no chão, altere a animação para `jump`
    mDrawComponent->SetAnimation("jump");
}

void Mario::Kill() {
    // --------------
    // TODO - PARTE 5
    // --------------

    // TODO 8 (~4 linhas): altere a animação para "dead" e o valor da variável `mIsDead` para verdadeiro.
    //  Além disso, desabilite os componentes `mRigidBodyComponent` e `mColliderComponent`
    mDrawComponent->SetAnimation("dead");
    mIsDead = true;
    mRigidBodyComponent->SetEnabled(false);
    mColliderComponent->SetEnabled(false);
}

void Mario::OnHorizontalCollision(const float minOverlap, AABBColliderComponent *other) {
    // --------------
    // TODO - PARTE 5
    // --------------

    // TODO 9 (~2-4 linhas): Verifique se a colisão ocorreu horizontalmente com um objeto do tipo
    //  `CollisionSide::Enemy`. Se sim, mate o jogador com o método `Kill()` do jogador.
    if (other->GetLayer() == ColliderLayer::Enemy) {
        Kill();
    }
}

void Mario::OnVerticalCollision(const float minOverlap, AABBColliderComponent *other) {
    switch(other->GetLayer()) {
        case ColliderLayer::Enemy: {
            other->GetOwner()->Kill();
            auto vel = mRigidBodyComponent->GetVelocity();
            mRigidBodyComponent->SetVelocity(Vector2(vel.x, mJumpSpeed / 2));
            return;
        }
        case ColliderLayer::Blocks: {
            if(minOverlap > 0) return;
            other->GetOwner()->OnVerticalCollision(minOverlap,mColliderComponent);
        }
    }
}
