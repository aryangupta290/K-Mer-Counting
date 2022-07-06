import matplotlib.pyplot as plt


# Using readlines()
file1 = open('input.txt', 'r')
Lines = file1.readlines()
 
x=[]
y=[]
count = 0
# Strips the newline character
for line in Lines:
    line.strip()
    t = line.split()
    x.append(float(t[0]))
    y.append(float(t[1]))


plt.xlabel('k-value')
plt.ylabel('Execution Time')

plt.plot(x,y)
plt.show()