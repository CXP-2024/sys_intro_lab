
with open('task1_grade_output.txt', 'r') as f:
    s = f.read()

N = 2048
s = s.split('\n')
try:
    a = [int(x.strip()) for x in s if x]
except ValueError as e:
    print('\033[0;31mFAILED:\033[0m Error: task1_grade_output.txt is not a valid integer list.')
    raise e
    exit(1)
assert len(a) == N, '\033[0;31mFAILED:\033[0m Error: output should have %d task_id(s), but only found %d'%(N, len(a))
assert sorted(a) == list(range(N)), '\033[0;31mFAILED:\033[0m Error: output is not a permutation of %d to %d'%(0, N-1)
print('\033[0;32mTask1 PASSED\033[0m')
