with open('input.txt', 'r') as f:
    num = 0
    sum = 0
    for line in f:
        for char in line:
            if char.isdigit():
                num = num*10 + int(char)
            else:
                sum = sum + num
                num = 0
            
print(sum)