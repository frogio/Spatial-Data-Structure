# Spatial Data Structure
Hash Table, Quad Tree, Kd Tree, OcTree를 구현해보고 </br>
Nearest Neighbour를 찾는 문제에 적용해보았다.


## 1. HashTable
동일한 해상도로 화면을 분할한 후 </br>
0 ~ 1 사이로 좌표를 정규화 한 다음 아래의 식으로 물체가 어느 grid에 속해 있는지 확인한다. </br>

grid_x = fmaxf(0, fminf(resolution - 1, resolution * x))</br>
grid_y = fmaxf(0, fminf(resolution - 1. resolution * y))</br>

검출된 grid 내와 그 주변 검색하여 최근접 이웃을 찾아낸다.
![image](https://github.com/user-attachments/assets/b9e3b706-8408-4445-9e58-835926362ca3)

## 2. QuadTree
가장 기본적인 4분 트리구조, </br>
BFS, DFS 방식으로 트리를 생성할 수 있는데, </br>
여기선 BFS 방식을 선택하여 트리를 생성했다. </br>

임계 깊이는 7, 임계 버텍스 개수는 2개로 정해, 입자의 임계치를 넘길경우 노드를 4분한다.
![image](https://github.com/user-attachments/assets/b41a99db-37ce-47e7-9a36-53e89a128c15)

## 3. Kd Tree
K dimensional Tree, QuadTree의 경우 Sparse한 입자들에 대해선 빈 노드들이 너무 생성되는 단점이 있다. </br>
QuadTree의 이러한 단점을 보안한 트리구조, </br>
노드를 분할할 때의 x, y 축을 번갈아 가 이분해 2진 트리로 구성하며 </br>
나누는 기준은 입자들의 좌표 중간값이다. </br>
마찬가지로 BFS 방식으로 트리를 생성한다.

![image](https://github.com/user-attachments/assets/d3c9cb26-5ba4-45b3-8856-d03c5f03dad8)

## 4. OcTree
QuadTree에서 z축을 추가한 8분 트리, </br>
생성 방식은 QuadTree와 동일하다. </br>

![image](https://github.com/user-attachments/assets/128bbcb4-ba8d-44ca-b140-2510d1037367)

## Nearest Neighbour Search 문제
QuadTree, Kd Tree에서 NN문제는 구성된 Tree를 BFS 방식으로 순회하여 범위 radius를 포함하거나 교차하는 영역의 노드를 탐색해 </br>
NN을 찾아냈다
