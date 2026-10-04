# Solução discreta da equação Eikonal

Considere um domínio quadrado discretizado por uma malha uniforme
$N \times N$. Cada nó é identificado por um par $(i,j)$, com
$0 \leq i,j < N$. Portanto, o número total de nós é

$$
M = N^2.
$$

Se necessário, a malha bidimensional pode ser associada a um vetor de
$M$ posições por meio do índice

$$
k = iN + j,
\qquad 0 \leq k < M.
$$

Essa associação é apenas uma forma de armazenar os nós. A discretização
da equação Eikonal continua sendo feita considerando os vizinhos
espaciais de cada nó.

## Equação contínua

Seja $T(x,y)$ o tempo de primeira chegada e $c(x,y)$ a velocidade da
onda. A equação Eikonal é

$$
\left\|\nabla T(x,y)\right\| = \frac{1}{c(x,y)}.
$$

Para uma malha com espaçamento $h$, escrevemos

$$
\left(\frac{\partial T}{\partial x}\right)^2
+ \left(\frac{\partial T}{\partial y}\right)^2
= \frac{1}{c(x,y)^2}.
$$

Denote por $U(i,j)$ a aproximação de $T(x_i,y_j)$ e por
$c(i,j)$ a velocidade no nó $(i,j)$.

## Aproximação upwind

Como a informação se propaga da fonte para os pontos ainda não
alcançados, devem ser usados os valores dos vizinhos que já possuem
tempo de chegada conhecido. Para o nó $(i,j)$, definimos

$$
a = \min\left(U(i-1,j), U(i+1,j)\right),
$$

considerando apenas os vizinhos horizontais aceitos, e

$$
b = \min\left(U(i,j-1), U(i,j+1)\right),
$$

considerando apenas os vizinhos verticais aceitos.

Os valores $a$ e $b$ representam os menores tempos aceitos nas duas
direções. A discretização upwind da equação Eikonal é

$$
\left(\frac{U(i,j)-a}{h}\right)^2
+ \left(\frac{U(i,j)-b}{h}\right)^2
= \frac{1}{c(i,j)^2}.
$$

Multiplicando por $h^2$ e definindo

$$
\tau(i,j) = \frac{h}{c(i,j)},
$$

obtemos

$$
\left(U(i,j)-a\right)^2
+ \left(U(i,j)-b\right)^2
= \tau(i,j)^2.
$$

Essa é a equação discreta que deve ser resolvida em cada atualização.

## Solução bidimensional

Expandindo os quadrados, obtemos uma equação de segundo grau:

$$
2U(i,j)^2
- 2(a+b)U(i,j)
+ a^2+b^2-\tau(i,j)^2 = 0.
$$

A raiz relevante é a maior raiz, pois o tempo de chegada no nó atual
deve ser maior ou igual aos tempos usados na atualização:

$$
U(i,j) =
\frac{a+b+\sqrt{2\tau(i,j)^2-(a-b)^2}}{2}.
$$

O discriminante pode ser escrito como

$$
\Delta = 2\tau(i,j)^2-(a-b)^2.
$$

A solução bidimensional só é válida quando

$$
|a-b| < \tau(i,j),
$$

que garante que a frente de onda está sendo atualizada com informação
das duas direções. Equivalentemente, nessa situação o valor calculado
satisfaz $U(i,j) \geq \max(a,b)$.

## Atualização unidimensional

Quando não há vizinhos aceitos nas duas direções, ou quando a condição
da solução bidimensional não é satisfeita, usa-se a atualização
unidimensional:

$$
U(i,j) = \min(a,b) + \tau(i,j).
$$

Se somente um dos valores estiver disponível, a fórmula é aplicada
usando esse único valor. Por exemplo, se apenas $a$ for conhecido,

$$
U(i,j) = a + \tau(i,j).
$$

Essa atualização representa a propagação da onda em uma única direção.

## Condição na fonte e nos contornos

Para uma fonte localizada no nó $(i_s,j_s)$, impõe-se

$$
U(i_s,j_s) = 0.
$$

Os demais nós começam com tempo infinito:

$$
U(i,j) = +\infty
\qquad \text{para } (i,j) \neq (i_s,j_s).
$$

Nos contornos da malha, são considerados apenas os vizinhos que
pertencem ao domínio. Por exemplo, para $i=0$ não existe o vizinho
$(i-1,j)$; nesse caso, somente o vizinho $(i+1,j)$ pode contribuir para
$a$.

## Algoritmo de atualização

Uma implementação do Fast Marching Method mantém três estados para cada
nó:

- `FAR`: o nó ainda não recebeu uma estimativa;
- `CONSIDERED`: o nó recebeu uma estimativa provisória;
- `ACCEPTED`: o menor tempo do nó já foi determinado.

Depois que um nó é aceito, seus vizinhos são atualizados pela expressão
discreta. O valor provisório é

$$
U_{\mathrm{novo}}(i,j) =
\begin{cases}
\dfrac{a+b+\sqrt{2\tau(i,j)^2-(a-b)^2}}{2},
& \text{se } |a-b| < \tau(i,j), \\[1.2em]
\min(a,b)+\tau(i,j),
& \text{caso contrário}.
\end{cases}
$$

O algoritmo escolhe, entre os nós `CONSIDERED`, aquele com menor tempo
provisório e o marca como `ACCEPTED`. Esse procedimento continua até
que não existam mais nós considerados.

## Complexidade

Como a malha possui $M=N^2$ nós, uma varredura completa para encontrar
o menor nó `CONSIDERED` custa $O(M)$. Se essa varredura for realizada
para cada nó, o custo total será

$$
O(M^2) = O(N^4).
$$

Usando uma fila de prioridade, a seleção do menor nó pode ser feita de
forma mais eficiente. Nesse caso, o custo típico do método é

$$
O(M\log M) = O(N^2\log N),
$$

dependendo da implementação da fila e do tratamento das atualizações.
