from sympy import *

t,x = symbols("t x")

tD, tP = symbols("tD tP")

w1, w2, w3 = symbols("w1 w2 w3")
a1, a2, a3 = symbols("a1 a2 a3")

gamma = symbols("gamma")
V0 = symbols("V0")

Vt = V0 + a1 * w1 + a2 * w2 + a3 * w3
E = gamma * Heaviside(Vt - tD)

E = gamma * Heaviside(Vt - tD) + Heaviside(tP - Vt)

E = gamma * Heaviside(Vt - tD)*(Vt - tD) + Heaviside(tP - Vt)*(tP - Vt)

print E.diff(w1)
print E.diff(w2)
print E.diff(w3)

