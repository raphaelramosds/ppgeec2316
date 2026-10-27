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

NOTE: na remoção, considere que
- A comparação apenas é justa se encad_remover buscar o nó da lista ligada que possui a chave que se quer remover. Assim, ambas fazem uma busca depois uma operação O(1)
- O encadeamento não depende de uma operação de realocar a tabela para suportar mais elementos, porque simplesmente podemos encadeá-los.

```
PLANO: comparar encadeamento com enderecamento aberto (probe linear)

busca (test_hash_search.c)
- vamos testar a busca para diferentes fatores de carga em uma tabela com m = 1024 slots fixos, de forma que n = alpha * m
- alpha in {0.1 .. 0.99, 1.0} (+ 2, 5 só encad), com sucesso x sem sucesso com m: 1024 (encad) e 1021 ou 1031 (aberto, primo)

insercao (test_hash_insert.c)
- forcar colisoes
- alpha in {0.1, 0.5, 0.9, 0.99} com m = 2^16 + 1 (aberto) e 2^16 (encad)

remocao (test_hash_remove.c)
- apenas remover: ambas tem o mesmo desempenho

ciclos (test_hash_remove_insert.c)
- ciclos remove+insere em alpha=0.5: busca sem sucesso ao longo dos ciclos. Aqui, remove-se varias vezes e depois buscar: o enderecamento aberto pode apresentar uma queda de perfomance, porque os DELETED vao ser visitados
```

15. Compare implementações de tabelas hash com hashing duplo e probing linear. Analise e justifique os resultados dos seus experimentos.

```
Tempo (ns) com tabela grande: a vantagem da sondagem linear

- Com m = 1021, a tabela ocupa 4 KB e cabe inteira na cache L1, então o tempo apenas acompanha as sondagens. 
- a sondagem linear acessa posições vizinhas na mesma linha de cache, enquanto o hashing duplo salta para posições aleatórias.

Chaves que colidem em h1 (pior caso)

- Inserir chaves da forma k = j·m + c (todas com o mesmo h1).
- Com sondagem linear elas formam um único agrupamento e a i-ésima inserção custa i sondagens, ou seja, Θ(n²) no total.
- Com hashing duplo, h2(k) = 1 + k mod (m−1) varia de chave para chave e as sequências de sondagem divergem. 
- Isso mostra que o hashing duplo elimina também o agrupamento secundário: chaves com o mesmo h1 deixam de seguir a mesma sequência.
```