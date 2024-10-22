import random

def generate_sample(name, postfix, dir):
    f = open('train/' + name + '.txt', 'r')
    res = f.readline()
    matrix = []
    for line in f:
        matrix.append(list(map(int, line.split())))
    f.close()
    noize = random.randint(1, 5)
    for _ in range(noize):
        x = random.randint(0, 6)
        y = random.randint(0, 6)
        matrix[x][y] = 1 - matrix[x][y]
    f = open(dir + '/' + name + '_' + str(postfix) + '.txt', 'w')
    f.write(res)
    for line in matrix:
        f.write(" ".join(list(map(str, line))))
        f.write('\n')
    f.close()

for i in range(20):
    generate_sample('circle', i, 'train')
    generate_sample('square', i, 'train')
    generate_sample('triangle', i, 'train')
    generate_sample('circle', i, 'validate')
    generate_sample('square', i, 'validate')
    generate_sample('triangle', i, 'validate')