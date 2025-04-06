# task2_grade_output.txt
# task2_grade_answer.txt
# task2_grade_time.txt
# task2_grade_baseline_time.txt
import os

N = 10 # test N times

if os.path.exists('/usr/bin/time'):
    cmd1 = '/usr/bin/time -p ./task2_grade_bin > task2_grade_output.txt 2> task2_grade_time.txt'
else:
    cmd1 = '/usr/bin/bash -c "time -p ./task2_grade_bin"  > task2_grade_output.txt 2> task2_grade_time.txt'
cmd2 = './baseline ./data/task2.txt ./task2_grade_answer.txt 2> task2_grade_baseline_time.txt'
cmd3 = './grade_checker ./task2_grade_answer.txt ./task2_grade_output.txt'

real_time = 0.
total_real_time = 0.
baseline_time = 0.
total_baseline_time = 0.

for i in range(N): 
    print('running baseline ...')
    if i == 0:
        print('it may takes minutes to complete...')
    assert os.system(cmd2)==0, 'run baseline error'
    print('running your code ...')
    assert os.system(cmd1)==0, 'run task2 error'
    assert os.system(cmd3)==0, 'run checker error'

    with open('task2_grade_time.txt', 'r') as f:
        s = f.read()
        s = s.split()
        assert s[0] == 'real'
        real_time = float(s[1])

    with open('task2_grade_baseline_time.txt', 'r') as f:
        s = f.read()
        baseline_time = float(s.split()[-1])
    
    total_real_time += real_time
    total_baseline_time += baseline_time

    s_up = (baseline_time/real_time)
    print(i+1,'/',N, '\033[0;32mSpeedup\033[0m: %.2f'%s_up , 'baseline: %.2fs'%baseline_time, 'yours: %.2fs'%real_time)

print('\033[0;32mAverage speedup\033[0m: %.2f'%(total_baseline_time/total_real_time) )
