# Projeto de Robotica Cognitiva

## Introdução

Este projeto é um protótipo de um robô móvel autónomo para patrulhamento. O robô tem dois modos de funcionamento. 
No modo de comissionamento, o robô explora o seu ambiente de trabalho e contrói um mapa, podendo fazê-lo de modo autónomo ou controlado a partir do teclado do computador. Neste modo é também possível selecionar quais os locais que se pretende que o robô patrulhe.
No modo de operação, o robô efetua uma rota de patrulha pelos pontos previamente definidos, autonomamente, de acordo com o planeamento de rota que o algoritmo adotado prevê. Durante a sua trajetória, o robô procura ainda identificar pessoas, averiguando se são pessoas desconhecidas à sua base de dados ou não. Em cada local de patrulha, o robô procura identifcar a adição ou remoção de objetos.

## Dependências

Este projeto foi feito em ROS1 Noetic e necessita da instalação dos seguintes pacotes (sudo apt install package_name):

```bash
sudo apt install ros-noetic-turtlebot3-msgs ros-noetic-turtlebot3-bringup ros-noetic-turtlebot3-slam ros-noetic-turtlebot3-navigation ros-noetic-joint-state-publisher-gui ros-noetic-navigation ros-noetic-gmapping ros-noetic-explore-lite
```

## Criação do workspace

Para a criação do workspace siga os seguintes passos:

Na pasta que quer por o projeto, abra uma consola e corra os seguintes comandos:

```bash

mkdir workspace_name
cd workspace_name

```

Copie os ficheiros para dentro do src folder no workspace_name, na consola verifique que está no workspace_name folder, e faça o comando:

```bash
-catkin_make
```

Espere que o computador compile o programa e corra o comando

```bash
source devel/setup.bash
```

## Correr o código

Primeiro correr:

```bash
roslaunch patrol exploration.launch
```

Espere até ver que a exploração acabou.

Depois pare o código e faça:

```bash
roslaunch patrol navigation.launch
```

Dentro do rviz utilize a função "Publish Point" para escolher os pontos de interesse para a patrulha, o robô irá agora fazer a patrulha desses pontos.

