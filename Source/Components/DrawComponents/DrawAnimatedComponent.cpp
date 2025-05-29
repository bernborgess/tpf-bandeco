//
// Created by Lucas N. Ferreira on 28/09/23.
//

#include "DrawAnimatedComponent.h"
#include "../../Actors/Actor.h"
#include "../../Game.h"
#include "../../Json.h"
#include <fstream>

DrawAnimatedComponent::DrawAnimatedComponent(class Actor *owner, const std::string &spriteSheetPath,
                                             const std::string &spriteSheetData, int drawOrder)
    : DrawSpriteComponent(owner, spriteSheetPath, 0, 0, drawOrder) {
    LoadSpriteSheet(spriteSheetPath, spriteSheetData);
}

DrawAnimatedComponent::~DrawAnimatedComponent() {
    for (const auto &rect: mSpriteSheetData) {
        delete rect;
    }
    mSpriteSheetData.clear();
}

void DrawAnimatedComponent::LoadSpriteSheet(const std::string &texturePath, const std::string &dataPath) {
    // Load sprite sheet texture
    mSpriteSheetSurface = mOwner->GetGame()->LoadTexture(texturePath);

    // Load sprite sheet data
    std::ifstream spriteSheetFile(dataPath);
    nlohmann::json spriteSheetData = nlohmann::json::parse(spriteSheetFile);

    SDL_Rect *rect = nullptr;
    for (const auto &frame: spriteSheetData["frames"]) {
        int x = frame["frame"]["x"].get<int>();
        int y = frame["frame"]["y"].get<int>();
        int w = frame["frame"]["w"].get<int>();
        int h = frame["frame"]["h"].get<int>();
        rect = new SDL_Rect({x, y, w, h});

        mSpriteSheetData.emplace_back(rect);
    }
}

void DrawAnimatedComponent::Draw(SDL_Renderer *renderer) {
    // --------------
    // TODO - PARTE 4
    // --------------

    // TODO 2.1 (1 linha): Todos os quadros desse objeto estão armazenados no vetor
    //  `mSpriteSheetData`. Cada posição desse vetor é um ponteiro para um SDL_Rect*, representando as coordenadas
    //  de um sprite no sprite sheet. Além disso, todas as animações estão armazenadas no mapa `mAnimations`.
    //  Uma animação é identificada por um nome (string) e definida por um vetor de índices de quadros
    //  (armazenados em mSpriteSheetData). A animação corrente é armazenada na variável membro `mAnimName`.
    //  Sempre que um objeto com o componente DrawAnimatedComponent é desenhado na tela,
    //  precisamos obter o índice do quadro corrente a partir do timer da animação. Para isso,
    //  basta converter o timer da animação (`mAnimTimer`) para inteiro. Obtenha o índice do quadro corrente
    //  indexando o mapa ` mAnimations` com o timer da animação (`mAnimTimer`) convertido para inteiro.
    //  Note que `mAnimations[mAnimName]` armazena os índices dos quadros da animação atual. Armazene
    //  o resultado em uma variável `spriteIdx`.
    int spriteIdx = mAnimations[mAnimName][int(mAnimTimer)];

    // TODO 2.2 (~7-10 linhas): Utilize a função SDL_RenderCopyEx para desenhar o sprite
    //  com índice `spriteIdx`. O SDLRect `srcRect` que define a região do sprite no sprite sheet está armazenado
    //  em `mSpriteSheetData[spriteIdx]`. Além disso, você terá que criar um SDL_Rect `dstRect` para definir a região
    //  da tela onde será desenhado o sprite, assim como no `DrawSpriteComponent`. Crie o `dstRect` considerando a
    //  altura e largura do sprite `srcRect`, não as propriedades `mWidth` and `mHeight`. Você também terá que
    //  criar uma flag do tipo SDL_RendererFlip assim como no DrawSpriteComponent.
    Vector2 pos = mOwner->GetPosition(),
            cam = mOwner->GetGame()->GetCameraPos();
    SDL_Rect *srcrect = mSpriteSheetData[spriteIdx];
    SDL_Rect dstrect = {.x = int(pos.x - cam.x), .y = int(pos.y - cam.y), .w = srcrect->w, .h = srcrect->h};
    auto flip = Math::NearZero(mOwner->GetRotation()) ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;
    SDL_RenderCopyEx(renderer, mSpriteSheetSurface, srcrect,
                     &dstrect, 0, nullptr, flip);
}

void DrawAnimatedComponent::Update(float deltaTime) {
    // --------------
    // TODO - PARTE 4
    // --------------

    // TODO 1.1 (~2 linhas): Verifique se animação está pausada (`mIsPaused`). Se estiver, saia da função (return).
    if (mIsPaused) return;

    // TODO 1.2 (~1 linha): Atualize o timer da animação `mAnimTimer` somando o tempo decorrido `deltaTime` multiplicado
    //  pela taxa de quadros por segundo `mAnimFPS`.
    mAnimTimer += deltaTime * mAnimFPS;

    // TODO 1.3 (~3-5 linhas): A função update deve converter o timer da animação `mAnimTimer` para um inteiro para obter
    //  o índice do quadro atual. No entanto, temos que garantir que esse índice não será maior do que número total de
    //  quadros da animação corrente (`mAnimations[mAnimName].size()). Verifique se o timer da animação é maior ou
    //  igual ao número de quadros da animação corrente. Se for, utilize um laço `while` para decrementar o timer por
    //  esse mesmo número até essa condição seja falsa.
    float size = mAnimations[mAnimName].size();
    while (mAnimTimer >= size) mAnimTimer -= size;
}

void DrawAnimatedComponent::SetAnimation(const std::string &name) {
    // --------------
    // TODO - PARTE 4
    // --------------

    // TODO 3 (~2 linhas): salve o nome da animação corrente `name` na variável membro `mAnimName` e
    //  chame a função Update passando delta time igual a zero para reinicializar o timer da animação
    //  `mAnimTimer`.
    mAnimName = name;
    Update(0);
}

void DrawAnimatedComponent::AddAnimation(const std::string &name, const std::vector<int> &spriteNums) {
    mAnimations.emplace(name, spriteNums);
}
