import numpy as np
import random
import datetime

tim = datetime.datetime.now()
randseed = tim.hour*10000+tim.minute*100+tim.second
random.seed(randseed)



# antall ledd i SVD som skal regnes ut
num = 4

# størrelse på matrisen
n = 5
p = 10

# matrisen består av tilfeldige tall mellom -1 og 1
A = np.empty((n, p))

for i in range(n):
    for j in range(p):
        A[i,j] = random.uniform(-1,1)

# setter en indeks til 100
A[2, 3] = 100
print (A)
U, sigma, Vt = np.linalg.svd(A)

SVD = np.zeros(np.shape(A)) 
if (n > len(sigma)):
    print("n er for stor")
else:
    for i in range(num):
        SVD += sigma[i] * np.outer(U[i], Vt[i])

print(SVD)