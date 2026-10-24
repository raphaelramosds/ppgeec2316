13. Implemente as seguintes estruturas de dados: 1) array ordenado, 2) lista encadeada não ordenada e 3) árvore de busca binária balanceada (por exemplo, AVL ou Red-Black). Em seguida, insira n elementos em cada estrutura de dados. Depois disso, considerando diferentes valores de n, compare os tempos de inserção e de busca de cada uma.

```
n          | array_inserir        lista_inserir        rn_inserir           | array_buscar         lista_buscar         rn_buscar
100000     | 141.291              1.688                14.732               | 0.0820               84.4500              0.1625
200000     | 526.582              2.401                34.718               | 0.0920               170.6485             0.1720
400000     | 2234.235             4.735                91.382               | 0.1000               391.9345             0.3090
800000     | 11043.184            9.476                259.773              | 0.1195               1154.8135            0.5505
1000000    | 18727.371            9.541                360.632              | 0.1210               1720.2400            0.4050
2000000    | 85907.593            22.892               1066.100             | 0.1385               3597.0635            0.8650
```

14. Implemente tabelas hash com encadeamento e com endereçamento aberto. Realize experimentos para demonstrar numericamente as vantagens e desvantagens de cada caso, considerando as operações de inserção, busca e remoção.

```

```

15. Compare implementações de tabelas hash com hashing duplo e probing linear. Analise e justifique os resultados dos seus experimentos.