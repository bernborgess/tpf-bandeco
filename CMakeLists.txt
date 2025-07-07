cmake_minimum_required(VERSION 3.26)
# Configurando o padrão c++
set(CMAKE_CXX_STANDARD 17)

# Configurando o nome e executável do projeto
set(PROJECT_NAME "tpf-bandeco")
project(${PROJECT_NAME})

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_mixer REQUIRED)

add_executable(${PROJECT_NAME}
        Source/Main.cpp
        Source/Math.cpp
        Source/Actors/Actor.cpp
        Source/Blocks/Block.cpp
        Source/Blocks/Block.h
        Source/Blocks/Deliver.cpp
        Source/Blocks/Deliver.h
        Source/Blocks/FoodBox.cpp
        Source/Blocks/FoodBox.h
        Source/Blocks/Stove.cpp
        Source/Blocks/Stove.h
        Source/Blocks/Table.cpp
        Source/Blocks/Table.h
        Source/Blocks/TableCut.cpp
        Source/Blocks/TableCut.h
        Source/Blocks/Trash.cpp
        Source/Blocks/Trash.h
        Source/Actors/Item.cpp
        Source/Actors/Item.h
        Source/Actors/Pan.cpp
        Source/Actors/Pan.h
        Source/Actors/Plate.cpp
        Source/Actors/Plate.h
        Source/Actors/Player.cpp
        Source/Actors/Player.h
        Source/Actors/Pot.cpp
        Source/Actors/Pot.h
        Source/Actors/ProgressBar.cpp
        Source/Actors/ProgressBar.h
        Source/AudioSystem.cpp
        Source/AudioSystem.h
        Source/Components/ColliderComponents/AABBColliderComponent.cpp
        Source/Components/ColliderComponents/AABBColliderComponent.h
        Source/Components/ColliderComponents/CircleColliderComponent.cpp
        Source/Components/ColliderComponents/CircleColliderComponent.h
        Source/Components/Component.cpp
        Source/Components/DrawComponents/DrawAnimatedComponent.cpp
        Source/Components/DrawComponents/DrawAnimatedComponent.h
        Source/Components/DrawComponents/DrawComponent.cpp
        Source/Components/DrawComponents/DrawComponent.h
        Source/Components/DrawComponents/DrawPolygonComponent.cpp
        Source/Components/DrawComponents/DrawPolygonComponent.h
        Source/Components/DrawComponents/DrawRectangleComponent.cpp
        Source/Components/DrawComponents/DrawRectangleComponent.h
        Source/Components/DrawComponents/DrawSpriteComponent.cpp
        Source/Components/DrawComponents/DrawSpriteComponent.h
        Source/Components/RigidBodyComponent.cpp
        Source/Components/RigidBodyComponent.h
        Source/CSV.h
        Source/Game.cpp
        Source/HUD.cpp
        Source/Json.h
        Source/Level.cpp
        Source/Level.h
        Source/OrderManager.cpp
        Source/OrderManager.h
        Source/Random.cpp
        Source/SpatialHashing.cpp
        Source/UIElements/UIButton.cpp
        Source/UIElements/UIButton.h
        Source/UIElements/UIElement.cpp
        Source/UIElements/UIElement.h
        Source/UIElements/UIFont.cpp
        Source/UIElements/UIFont.h
        Source/UIElements/UIImage.cpp
        Source/UIElements/UIImage.h
        Source/UIElements/UIScreen.cpp
        Source/UIElements/UIScreen.h
        Source/UIElements/UIText.cpp
        Source/UIElements/UIText.h
)

target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2 SDL2_image::SDL2_image SDL2_ttf::SDL2_ttf SDL2_mixer::SDL2_mixer)
