#recursive function
#실습 환경은 colab or vscode recommend
def evaluate_postfix(tokens):
    if not tokens:
        return None
    token=tokens.pop()
    if token.isnumeric():
        return float(token)
    else:
        operation2=evaluate_postfix(tokens)
        operation1=evaluate_postfix(tokens)
        if token=="+":
            return operation1+operation2
        elif token=="-":
            return operation1-operation2
        elif token=="*":
            return operation1*operation2
        elif token=="/":
            return operation1/operation2
        elif token=="^":
            return operation1**operation2
postfix="2 3 4 + *"
tokens=postfix.split()
result=evaluate_postfix(tokens.copy())
print(result)


#연습문제 1
def sen(n):
    if n==1:
        return 1
    else:
        return  sen(n-1)+3
for i in range(1,11):
    print(sen(i))

#연습문제 2(hanoi)//think about it!
def move(n,src,tmp,spane):
    print("n:",n,"src:",src,"tmp:",tmp,"spane:",spane)
    if n==1:
        print(src,"->",spane)
    else:
        move(n-1,src,spane,tmp)
        print(src,"->",spane)
        move(n-1,tmp,src,spane)
move(3,"A","B","C")

#연습문제 3 (pow)
def pow(x,n):
    if n==0:
        return 1
    else:
        return x*pow(x,n-1)
print(pow(3,3))

#연습문제 4 (find max)
num=[2,8,10,7,6,1,31,9]
n=len(num)
def find_max(num,n):
    if n==1:
        return num[0]
    else:
        max=find_max(num,n-1)
        if max>num[n-1]:
            return max
        else:
            return num[n-1]
print(find_max(num,n))
