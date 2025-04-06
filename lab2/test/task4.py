# Test task4
print('warning')
print('this tester is only suit for task4 demo data')
print('the answer is hard coded')

with open('task4_output.txt', 'r') as f:
    output = f.readlines()

task_id, M, N = output[0].split()

M = int(M)
N = int(N)

assert M == 512
assert N == 128

xor_sum = 0
for line in output[1:]:
    nums = line.split()
    for num in nums:
        xor_sum ^= int(num)

# print(xor_sum)
assert xor_sum == 122612008

print('\033[0;32mTask4 PASSED\033[0m')

