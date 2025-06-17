//
// Created by Lucas N. Ferreira on 28/09/23.
//

#include "AABBColliderComponent.h"

#include <complex>

#include "../../Actors/Actor.h"
#include "../../Game.h"

AABBColliderComponent::AABBColliderComponent(class Actor *owner, int dx, int dy,
                                             int w, int h, ColliderLayer layer,
                                             bool isStatic, int updateOrder)
    : Component(owner, updateOrder),
      mOffset(Vector2((float)dx, (float)dy)),
      mWidth(w),
      mHeight(h),
      mIsStatic(isStatic),
      mLayer(layer) {
    mOwner->GetGame()->AddCollider(this);
}

AABBColliderComponent::~AABBColliderComponent() {
    mOwner->GetGame()->RemoveCollider(this);
}

Vector2 AABBColliderComponent::GetMin() const {
    // --------------
    // TODO - PARTE 3
    // --------------

    // TODO 1.1 (1 linha): Calcule (e retorne) o ponto mínimo dessa AABB. A
    // variável `mOffset`
    //  define a posição da AABB com relação a posição do objeto dono do
    //  componente. Portanto, basta somar a posição do objeto dono do componente
    //  a esse deslocamento.
    return mOffset + mOwner->GetPosition();
}

Vector2 AABBColliderComponent::GetMax() const {
    // --------------
    // TODO - PARTE 3
    // --------------

    // TODO 1.2 (1 linha): Calcule (e retorne) o ponto máximo dessa AABB. As
    // variáveis membro
    //  `mWidth` e `mHeight` definem a altura e a largura da AABB,
    //  respectivamente. Portanto, basta somar a largura à coordenada x e a
    //  altura à coordenada y do ponto mínimo da AABB (utilize o método `GetMin`
    //  implementado na etapa anterior).
    return GetMin() + Vector2(mWidth, mHeight);
}

bool AABBColliderComponent::Intersect(const AABBColliderComponent &b) const {
    // --------------
    // TODO - PARTE 3
    // --------------

    // TODO 2 (~5 linhas): Verifique se esta AABB está colidindo com a AABB b
    // passada como parâmetro.
    //  Retorne verdadeiro se estiver e falso caso contrário. Utilize os métodos
    //  `GetMin` e `GetMax` para acessar os pontos de mínimo e máximo das duas
    //  AABBs. É importante destacar que quando os valores foram iguais, as
    //  AABBs NÃO estão colidindo. Portanto, utilize o operador < para verificar
    //  se as AABBs estão colidindo.
    auto Amax = GetMax();
    auto Amin = GetMin();
    auto Bmax = b.GetMax();
    auto Bmin = b.GetMin();
    return !(Amax.x < Bmin.x || Bmax.x < Amin.x || Amax.y < Bmin.y ||
             Bmax.y < Amin.y);
}

float AABBColliderComponent::GetMinVerticalOverlap(
    AABBColliderComponent *b) const {
    // --------------
    // TODO - PARTE 3
    // --------------

    // TODO 3.1 (2 linhas): Calcule as interseções verticais (top e down) entre
    // as duas AABBs.
    auto Amax = GetMax();
    auto Amin = GetMin();
    auto Bmax = b->GetMax();
    auto Bmin = b->GetMin();

    float d_top = Amin.y - Bmax.y;
    float d_down = Amax.y - Bmin.y;

    // TODO 3.2 (1 linha): Encontre e retorne a interseção com menor valor
    // absoluto.
    return Math::Abs(d_top) < Math::Abs(d_down) ? d_top : d_down;
}

float AABBColliderComponent::GetMinHorizontalOverlap(
    AABBColliderComponent *b) const {
    // --------------
    // TODO - PARTE 3
    // --------------

    // TODO 3.3 (2 linhas): Calcule as interseções horizontais (right e left)
    // entre as duas AABBs.
    auto Amax = GetMax();
    auto Amin = GetMin();
    auto Bmax = b->GetMax();
    auto Bmin = b->GetMin();

    float d_left = Amin.x - Bmax.x;
    float d_right = Amax.x - Bmin.x;

    // TODO 3.4 (1 linha): Encontre e retorne a interseção com menor valor
    // absoluto.
    return Math::Abs(d_left) < Math::Abs(d_right) ? d_left : d_right;
}

float AABBColliderComponent::DetectHorizontalCollision(
    RigidBodyComponent *rigidBody) {
    // --------------
    // TODO - PARTE 3
    // --------------
    if (mIsStatic) return false;

    for (auto collider : mOwner->GetGame()->GetColliders()) {
        // TODO 4.1: Percorra todos os colliders e verifique se o objeto dono do
        // componente
        //  está colidindo com algum deles.
        // Antes dessa verificação, verifique se o collider está habilitado
        //  e se não é o próprio collider do objeto dono do componente. Em ambos
        //  os casos, ignore o collider e continue a verificação.
        if (!collider->mIsEnabled) continue;
        if (collider->mOwner == mOwner) continue;

        // Se o objeto dono do componente estiver colidindo horizontalmente com
        // algum collider,
        //  resolva a colisão horizontal e retorne o valor da interseção
        //  horizontal mínima.
        if (!Intersect(*collider)) continue;

        // Utilize o método `GetMinHorizontalOverlap` para calcular a interseção
        // horizontal mínima.
        auto mh = GetMinHorizontalOverlap(collider);
        auto mv = GetMinVerticalOverlap(collider);
        if (Math::Abs(mh) >= Math::Abs(mv)) continue;

        // Utilize o método `ResolveHorizontalCollisions` para resolver a
        // colisão horizontal.
        ResolveHorizontalCollisions(rigidBody, mh);
        // Utilize o método `OnHorizontalCollision` do objeto dono do componente
        // para notificar a colisão ao Actor dono desse componente.
        rigidBody->GetOwner()->OnHorizontalCollision(mh, collider);
        return mh;
    }

    return 0.0f;
}

float AABBColliderComponent::DetectVerticalCollision(
    RigidBodyComponent *rigidBody) {
    // --------------
    // TODO - PARTE 3
    // --------------
    if (mIsStatic) return false;

    auto colliders = mOwner->GetGame()->GetColliders();

    // TODO 4.2: Percorra todos os colliders e verifique se o objeto dono do
    // componente
    //  está colidindo com algum deles. Antes dessa verificação, verifique se o
    //  collider está habilitado e se não é o próprio collider do objeto dono do
    //  componente. Em ambos os casos, ignore o collider e continue a
    //  verificação. Se o objeto dono do componente estiver colidindo
    //  verticalmente com algum collider, resolva a colisão vertical e retorne o
    //  valor da interseção vertical mínima. Utilize o método
    //  `GetMinVerticalOverlap` para calcular a interseção vertical mínima.
    //  Utilize o método `ResolveVerticalCollisions` para resolver a colisão
    //  vertical. Utilize o método `OnVerticalCollision` do objeto dono do
    //  componente para notificar a colisão ao Actor dono desse componente.
    for (auto collider : mOwner->GetGame()->GetColliders()) {
        if (!collider->mIsEnabled) continue;
        if (collider->mOwner == mOwner) continue;
        if (!Intersect(*collider)) continue;

        auto mh = GetMinHorizontalOverlap(collider);
        auto mv = GetMinVerticalOverlap(collider);
        if (Math::Abs(mv) > Math::Abs(mh)) continue;

        ResolveVerticalCollisions(rigidBody, mv);
        rigidBody->GetOwner()->OnVerticalCollision(mv, collider);
        return mv;
    }

    return 0.0f;
}

void AABBColliderComponent::ResolveHorizontalCollisions(
    RigidBodyComponent *rigidBody, const float minXOverlap) {
    // --------------
    // TODO - PARTE 3
    // --------------

    // TODO 5.1 (2 linhas): Aplique a resolução de colisão horizontal. Para
    // isso, mova o objeto dono do componente
    //  para o lado oposto da colisão. Para isso, subtraia o valor da interseção
    //  horizontal mínima da posição do objeto dono do componente. Utilize o
    //  método `SetPosition` do objeto dono do componente. Altere também a
    //  velocidade horizontal do objeto dono do componente para 0.0f. Utilize o
    //  método `SetVelocity`.
    auto [ox, oy] = mOwner->GetPosition();
    mOwner->SetPosition(Vector2(ox - minXOverlap, oy));
    auto [_, dy] = rigidBody->GetVelocity();
    rigidBody->SetVelocity(Vector2(0, dy));
}

void AABBColliderComponent::ResolveVerticalCollisions(
    RigidBodyComponent *rigidBody, const float minYOverlap) {
    // --------------
    // TODO - PARTE 3
    // --------------

    // TODO 5.2 (2 linhas): Aplique a resolução de colisão vertical. Para isso,
    // mova o objeto dono do componente
    //  para o lado oposto da colisão. Para isso, subtraia o valor da interseção
    //  vertical mínima da posição do objeto dono do componente. Utilize o
    //  método `SetPosition` do objeto dono do componente. Altere também a
    //  velocidade vertical do objeto dono do componente para 0.0f. Utilize o
    //  método `SetVelocity`.
    auto [ox, oy] = mOwner->GetPosition();
    mOwner->SetPosition(Vector2(ox, oy - minYOverlap));
    auto [dx, _] = rigidBody->GetVelocity();
    rigidBody->SetVelocity(Vector2(dx, 0));
}
