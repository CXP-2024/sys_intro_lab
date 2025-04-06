import random, os

with open('data/task4_articles.txt', 'r') as f:
    articles = f.read()

lines = []
file_names = 'Azure Black Blue Brown Cyan Green Gray Magenta Orange Pink Purple Red White'.split()

def preprocess():
    s = articles.replace(',', ',\n')
    s = s.replace('.', '.\n')
    global lines
    lines = articles.split('\n')
    lines = [x.strip() for x in lines if x.strip()]
    print(len(lines))

def gen_input():
    data_lines = []
    M, K, N = 512, 256, 128
    MN, MX = 0, 1024
    task4_input = '1\n1 %d %d %d\n'%(M, K, N)

    for i in range(M):
        prefix = 'DATA_A_%d: '%i
        nums = [str(random.randint(MN, MX)) for i in range(K)]
        task4_input += ' '.join(nums)+'\n'
        splitter = ', '
        if random.randint(0,1)==0:
            splitter = ' '
        s = prefix + splitter.join(nums)
        data_lines.append(s)
    for i in range(K):
        prefix = 'DATA_B_%d: '%i
        nums = [str(random.randint(MN, MX)) for i in range(N)]
        task4_input += ' '.join(nums)+'\n'
        splitter = ','
        if random.randint(0,1)==0:
            splitter = ' '
        s = prefix + splitter.join(nums)
        data_lines.append(s)
    
    with open('data/task4_input.txt', 'w') as f:
        f.write(task4_input)

    random.shuffle(data_lines)
    N = len(file_names)
    block = (len(data_lines)-1)//N +1
    print(N, block)
    for i in range(N):
        file_name = file_names[i]
        data_part = data_lines[i*block: (i+1)*block]
        rand_lines = random.sample(lines, random.randint(len(data_part)*2, len(data_part)*3))
        data_part.extend(rand_lines)
        random.shuffle(data_part)
        with open('data/task4/%s.txt'%file_name, 'w') as f:
            f.write('\n'.join(data_part))

def gen_data_source():
    url = 'http://people.iiis.tsinghua.edu.cn/~weixu/Krvdro9c/sysintro_task4.tar.gz'
    with open('data/task4_datasource_template.txt', 'r') as f:
        s = f.read()
    with open('data/data_source.txt', 'w') as f:
        f.write(s.format(url))

if __name__ == '__main__':
    random.seed(42)
    preprocess()
    if not os.path.exists('data/task4'):
        os.mkdir('data/task4')

    gen_data_source()
    gen_input()