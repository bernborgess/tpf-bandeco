# 🎮 Overcooked: Pesadelo no Bandeco

## Screenshots
- O menu principal
![Screenshot 2025-06-30 11 47 55 PM](https://github.com/user-attachments/assets/650f0b39-89bd-4fe7-85de-45aca2850dcf)

- Como jogar?
 ![Screenshot 2025-06-30 11 48 09 PM](https://github.com/user-attachments/assets/b58ee511-74c6-4e83-abec-4a8765284ea6)

- O começo da fase 1
![Screenshot 2025-06-30 11 48 29 PM](https://github.com/user-attachments/assets/aab98215-18eb-4af6-bd50-c04057d31291)

- Cortando tomates
![Screenshot 2025-06-30 11 49 02 PM](https://github.com/user-attachments/assets/0396af6e-57b2-4c51-a0b6-fec4127fe055)

- Fazendo sopa de tomate
![Screenshot 2025-06-30 11 55 42 PM](https://github.com/user-attachments/assets/c9e34fc0-2fed-48d0-8bab-891c77c0f02c)

- Entregando o pedido
![Screenshot 2025-06-30 11 56 07 PM](https://github.com/user-attachments/assets/005b3aad-a99d-42b3-b8b2-9c6c0a447672)

- Descartando a sopa queimada
![Screenshot 2025-06-30 11 49 38 PM](https://github.com/user-attachments/assets/89da47c0-5721-497f-b08b-22c1efde2491)

- Fim do tempo
![Screenshot 2025-06-30 11 57 40 PM](https://github.com/user-attachments/assets/b17b75a9-6298-4cec-8bc8-11c5d837cc71)

- Resultados da fase
![Screenshot 2025-06-30 11 57 49 PM](https://github.com/user-attachments/assets/646fcc69-c5d4-44c3-b0ad-697dc96c4b0f)



## Descrição
<!-- TODO: 3 parágrafos -->
<!-- TODO: objetivo, mecânicas básicas, condições de vitória e derrota -->
 O jogo **Overcooked: Pesadelo no Bandeco** é inspirado em
[Overcooked!](https://www.team17.com/games/overcooked) de team17, um jogo
cooperativo de 2 jogadores baseado nas áreas de alimentação da UFMG, focado em
cozinhar e entregar pratos sob pressão.

Os jogadores são colocados em diferentes mapas onde devem cozinhar os pedidos o
mais rápido possível, superando condições adversas e comunicando-se para
conseguir coordenar e dividir tarefas.  Na primeira fase os jogadores tem a receita de sopa de tomate, que envolve pegar tomates da caixa, cortar tomates sobre a bancada e adicionar 3 tomates cortados a panela. O cozimento deve ser feito com atenção, pois se passar muito do tempo a sopa irá queimar e o prato tem que ser descartado.

Para vencer a fase os
jogadores deve ser capazes de entregar uma quantidade satisfatória de pratos.
Caso isso não ocorra os jogadores deverão tentar novamente. Caso consigam, podem
prosseguir para novos restaurantes.

## Instalação
### Debian
- Em um terminal bash, execute os comandos a seguir
```bash
# Install git and dev tools if not available
sudo apt install build-essential git make pkg-config cmake ninja-build gnome-desktop-testing libasound2-dev libpulse-dev libaudio-dev libjack-dev libsndio-dev libx11-dev libxext-dev libxrandr-dev libxcursor-dev libxfixes-dev libxi-dev libxss-dev libxtst-dev libxkbcommon-dev libdrm-dev libgbm-dev libgl1-mesa-dev libgles2-mesa-dev libegl1-mesa-dev libdbus-1-dev libibus-1.0-dev libudev-dev libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev libsdl2-ttf-dev
# Clone this repository
git clone https://github.com/bernborgess/tpf-bandeco
cd tpf-bandeco
# Configure the build
mkdir build
cd build
cmake ..
make -j$(nproc)
# Run the game!
./tpf-bandeco 
```

## Funcionalidades a serem testadas
<!-- TODO: Lista de funcionalidades que os jogadores devem observar com atenção durante o Playtesting -->
1. Aprender os controles e objetivo do jogo, pela tela `Como Jogar?`
1. Iniciar uma nova fase
1. Pegar ingredientes na caixa de tomates
1. Cortar tomates na tábua de corte
1. Fazer uma sopa na panela com três tomates cortados
1. Pegar uma sopa pronta da panela com um prato
1. Entregar um prato aos clientes no block das "setas"
1. Deixar uma sopa queimar por ficar muito tempo no fogo
1. Descartar a sopa queimada na lixeira


## Créditos
<!-- TODO: Mais detalhes -->
- Bernardo Borges - Programação da Lógica de Items e de Blocos
- Daniele Cassia - Design e Criação dos Blocos e sprites


