import cs50

def main():
    change = int(get_positive_float() * 100)
    
    coinCount = 0
    
    #for i in range(0,change):
    while (change > 0):
        if ((change - 25) >= 0):
            change -= 25
            coinCount += 1
        elif ((change - 10) >= 0):
            change -= 10
            coinCount += 1
        elif ((change - 5) >= 0):
            change -= 5
            coinCount += 1
        elif ((change - 1) >= 0):
            change -= 1
            coinCount += 1
    print(coinCount)

def get_positive_float():
    while True:
        print("O hai! How much change is owed?")
        n = cs50.get_float()
        if 0 < n:
            break
    return n
        
if __name__ == "__main__":
    main()