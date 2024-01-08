# Projeto de Robotica Cognitiva

## Introdução

Este projeto é um protótipo de um robô móvel autónomo para patrulhamento, até agora o robô é capaz de explorar o espaço, mapear e de seguida receber pontos para patrulha, conseguindo escolher para onde ir e como lá chegar.

## Dependências

Este projeto foi feito em ROS1 Noetic e precisa dos seguintes packages (sudo apt install package_name):

```bash
sudo apt install ros-noetic-turtlebot3-msgs ros-noetic-turtlebot3-bringup ros-noetic-turtlebot3-slam ros-noetic-turtlebot3-navigation ros-noetic-joint-state-publisher-gui ros-noetic-navigation ros-noetic-gmapping ros-noetic-explore-lite
```

## Criação do workspace

Para a criação do workspace siga os seguintes passos:

Na pasta que quer por o projeto,

```bash

mkdir workspace_name
cd workspace_name

```

Copia os ficheiros para dentro do src folder no workspace_name, na consola verifica que está no workspace_name folder, e faz o comando:

```bash
-catkin_make
```

Espera que o computador compile o programa e corre o comando

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

