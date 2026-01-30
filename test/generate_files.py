import random

def make_file(filename, lines):
    with open(filename, "w") as f:
        for _ in range(lines):
            f.write(str(random.randint(1, 25)) + "\n")

make_file("file_100.txt", 100)
make_file("file_10000.txt", 10000)
