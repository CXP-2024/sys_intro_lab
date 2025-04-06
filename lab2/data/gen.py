import random
def rand_matrix(m, n, a, b):
    s = ''
    for i in range(m):
        for j in range(n):
            s += '%d '%random.randint(a, b)
        s += '\n'
    return s

def gen_task2():
    s = ''
    num = 100
    s += '%d\n'%num
    num1 = int(num*0.9)
    num2 = num-num1
    for i in range(num1):
        print('Generating task2 data: ',i+1, '/', num)
        task_id = i+1
        m = random.randint(256, 512)
        k = random.randint(128, 256)
        n = random.randint(256, 512)
        s += '%d %d %d %d\n'%(task_id, m, k, n)
        s += rand_matrix(m, k, 0, 255)
        s += rand_matrix(k, n, 0, 255)
    
    for i in range(num2):
        print('Generating task2 data: ',i+1+num1, '/', num)
        task_id = i+1 + num1
        m = 768
        k = m // 2
        # k = random.randint(128, 256)
        n = 768
        s += '%d %d %d %d\n'%(task_id, m, k, n)
        s += rand_matrix(m, k, 0, 255)
        s += rand_matrix(k, n, 0, 255)

    with open('data/task2.txt', 'w') as f:
        f.write(s)



def gen_task3():
    s = ''
    num = 20 + 20
    s += '%d\n'%num
    for i in range(num//2):
        print('Generating task3 data: ',i+1, '/', num)
        task_id = i+1
        m = random.randint(512, 1024)
        k = random.randint(256, 512)
        n = random.randint(512, 1024)
        s += '%d %d %d %d\n'%(task_id, m, k, n)
        s += rand_matrix(m, k, 0, 999)
        s += rand_matrix(k, n, 0, 999)
    
    for i in range(num//2):
        print('Generating task3 data: ',i+1 + num//2, '/', num)
        task_id = i+1 + num//2
        m = random.randint(1, 3)
        k = random.randint(512, 512)
        n = random.randint(256, 512)
        s += '%d %d %d %d\n'%(task_id, m, k, n)
        s += rand_matrix(m, k, 0, 999)
        s += rand_matrix(k, n, 0, 999)

    with open('data/task3.txt', 'w') as f:
        f.write(s)

def main():
    random.seed(42)
    gen_task2()
    gen_task3()

if __name__=='__main__':
    main()