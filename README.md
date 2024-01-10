# Projeto de Robótica Cognitiva

## Introdução

Este projeto é um protótipo de um robô móvel autónomo para patrulhamento. O robô tem dois modos de funcionamento. 
No modo de comissionamento, o robô explora o seu ambiente de trabalho e contrói um mapa, podendo fazê-lo de modo autónomo ou controlado a partir do teclado do computador. Neste modo é também possível selecionar quais os locais que se pretende que o robô patrulhe.
No modo de operação, o robô efetua uma rota de patrulha pelos pontos previamente definidos, autonomamente, de acordo com o planeamento de rota que o algoritmo adotado prevê. Durante a sua trajetória, o robô procura ainda identificar a presença de pessoas, alertando o utilizador. Em cada local de patrulha, o robô procura identificar a adição ou remoção de objetos entre patrulhas a esse local.

## Dependências

Este projeto foi feito em ROS1 Noetic e necessita da instalação dos seguintes pacotes:
POR ATUALIZAR
```bash
sudo apt install ros-noetic-turtlebot3-msgs ros-noetic-turtlebot3-bringup ros-noetic-turtlebot3-slam ros-noetic-turtlebot3-navigation ros-noetic-joint-state-publisher-gui ros-noetic-navigation ros-noetic-gmapping ros-noetic-explore-lite
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
* 1
* 2
* 3

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
* 1
* 2
* 3
