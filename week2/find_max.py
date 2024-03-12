#if user enter numbers, return them

def find_max(num,n):
    if n==1:
        return num[0]
    else:
        if find_max(num,n-1)>num[n-1]:
            return find_max(num,n-1)
        else:
            return num[n-1]

num = []
for i in range(0, 3):
    number = int(input("Enter a %d th number: " % (i + 1)))  # 입력값을 정수로 변환
    num.append(number)
print(find_max(num, len(num)))
