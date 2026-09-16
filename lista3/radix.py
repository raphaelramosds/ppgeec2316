# reference: https://www.geeksforgeeks.org/dsa/radix-sort/

a = [170, 45, 75, 90, 802, 24, 2, 66]

def encontrar_maior(a):
    n = len(a)
    maior = a[0]
    for i in range(1,n):
        el = a[i]
        if el <= maior:
            continue
        maior = el
    return maior