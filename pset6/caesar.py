import cs50
import sys

if len(sys.argv) != 2:
    print("BZZZ! WRONG. ONE KEY ONLY.")
    exit(1)

key = int(sys.argv[1])

plain = input("plaintext: ")

print("ciphertext: ", end='')

# iterates through characters in the plaintext input
for i in range(len(plain)):
    
    # converts each plaintext character string to its ascii code
    num = ord(plain[i])
    
    # moves the ascii code by the user-specified key
    if 65 <= num <= 90:
        if ((num + key) % 90) == (num + key):
            num2 = num + key
        else:
            num2 = 65 + ((num + key) % 90) - 1
    elif 97 <= num <= 122:
        if ((num + key) % 122) == (num + key):
            num2 = num + key
        else:
            num2 = (97 + ((num + key) % 122)) - 1
    else:
        num2 = num

    # converts the result of the move from ascii to a character string
    cipher = chr(num2)
    # prints the character in a sequence
    print(cipher, end='')
    #increments the iterator
    i += 1

print('\n')