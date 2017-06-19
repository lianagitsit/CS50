import cs50

def main():
    height = get_positive_int()
    
    for row in range(height):
        space = height - (row + 1)
        brick = row + 1
        print(" " * space, "#" * brick)

def get_positive_int():
    while True:
        print("Height: ", end="")
        n = cs50.get_int()
        if 0 < n < 23:
            break
    return n

if __name__ == "__main__":
    main()