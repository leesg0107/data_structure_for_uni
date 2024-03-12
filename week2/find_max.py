#if user enter numbers, return them

def find_max_recursive(num,n):
    if n==1:
        return num[0]
    else:
        if find_max_recursive(num,n-1)>num[n-1]:
            return find_max_recursive(num,n-1)
        else:
            return num[n-1]

def find_max_for(num,n):
    max=num[0]
    for i in range(1,n):
        if max<num[i]:
            max=num[i]
        else:
            max=max
    return max

num = []
for i in range(0, 10):
    number = float(input("Enter a %d th number: " % (i + 1)))
    num.append(number)

print(find_max_recursive(num, len(num)))
print(find_max_for(num, len(num)))
