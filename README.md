# Projeto de Robótica Cognitiva

## Introdução

Este projeto é um protótipo de um robô móvel autónomo para patrulhamento. O robô tem dois modos de funcionamento. 
No modo de comissionamento, o robô explora o seu ambiente de trabalho e contrói um mapa, podendo fazê-lo de modo autónomo ou controlado a partir do teclado do computador. Neste modo é também possível selecionar quais os locais que se pretende que o robô patrulhe.
No modo de operação, o robô efetua uma rota de patrulha pelos pontos previamente definidos, autonomamente, de acordo com o planeamento de rota que o algoritmo adotado prevê. Durante a sua trajetória, o robô procura ainda identificar a presença de pessoas, alertando o utilizador. Em cada local de patrulha, o robô procura identificar a adição ou remoção de objetos entre patrulhas a esse local.

## Dependências

Este projeto foi feito em ROS1 Noetic e necessita da instalação dos seguintes pacotes:
```bash
sudo apt install ros-noetic-gmapping libuvc-dev ros-noetic-explore-lite ros-noetic-navigation libaria-dev ros-noetic-turtlebot3-description

sudo apt install ros-noetic-vision-msgs python3-pip
pip install -r requirements.txt
pip install numpy
pip install python-dateutil
pip install scipy
```

Pode ser necessário correr ao seguinte comando para aceder ao robô.
```bash
sudo adduser user dialout
```

## Criação do workspace

Para a criação do workspace siga os seguintes passos:

Na pasta em que pretende colocar o projeto, abra uma consola e corra os seguintes comandos:

```bash

mkdir workspace_name
cd workspace_name
git clone https://github.com/DavidFerreira1002/RC_Project.git
```

Após o término da transferência, e na pasta workspace_name, corra o seguinte comando:

```bash
catkin_make
```

Espere que o computador compile o programa e, por fim, corra o comando:

```bash
source devel/setup.bash
```

## Executar o código

### Modo de comissionamento

O código pode ser corrido em ambiente de simulação Gazebo ou com ligação a um robô Pioneer 3 - DX.
No caso da simulação, deve correr: 

```bash
roslaunch patrol comissioning.launch
```

Já no caso da ligação ao robô deve correr, em terminais diferentes da consola, estes comandos:

```bash
roslaunch robot robot.launch
roslaunch patrol comissioning_real.launch
```

Quando receber uma mensagem no terminal, o utilizador pode passar à edição do mapa. Dentro do rviz, utilize a função "Publish Point" para escolher os pontos de interesse para a patrulha. Quando todos os pontos de interesse estiverem escolhidos, pode fechar o launch file. 


Argumentos que o utilizador deve definir em `comissioning.launch` e/ou `comissioning_real.launch`:
* automatic_exploration -> define se a exploração é manual ou automática;
* world_name -> define o nome do mapa;

### Modo de operação

Para inicializar o modo de operação, deve correr os códigos seguintes.
No caso da simulação, deve correr:

```bash
roslaunch patrolling_sim patrolling.launch
```

Já no caso da ligação ao robô, deve correr, em terminais diferentes da consola, estes comandos:

```bash
roslaunch robot robot.launch (caso o robô ainda não tenha sido inicializado)
roslaunch patrolling_sim patrolling_real.launch
```

O robô planeia então uma rota de patrulha tendo em conta os pontos de interesse selecionados anteriormente e autonomamente desloca-se segundo o plano, tendo em conta a presença de pessoas e a adição ou remoção de objetos nos pontos de interesse da patrulha.

Argumentos que o utilizador deve definir em `patrolling.launch` e/ou `patrolling_real.launch`:
* prior_map -> define o nome do mapa a ser usado;
* algorithm -> define o algoritmo de planeamento de rota a ser utilizado;

É necessário correr também alguns nodos para verificar a funcionalidade de reconhecimento de objetos e de pessoas, lançados correndo o seguinte launch file:

```bash
roslaunch yolo_handler yolo.launch 
```

Este ficheiro lança o nodo de análise de imagem através do algoritmo YOLOv7, e o algoritmo de análise dos objetos encontrados para as funcionalidades de adição e remoção de objetos, e o reconhecimento de pessoas no mapa entre locais a vigiar.

