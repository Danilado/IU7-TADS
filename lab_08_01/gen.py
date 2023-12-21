from random import randint

nodes = int(input("Введите количество узлов: "))
paths = int(input("Введите количество путей: "))

connections = [[] for _ in range(nodes)]

for i in range(paths):
    node = randint(0, nodes - 1)
    path = randint(0, nodes - 1)

    while path == node or path in connections[node]:
        node = randint(0, nodes - 1)
        path = randint(0, nodes - 1)

    connections[node].append(path)

with open(f"./graph_{nodes}_{paths}.txt", "w") as f:
    f.write(f"{nodes}\n")
    for node, cons in enumerate(connections):
        for con in cons:
            f.write(f"{node} {con}\n")
