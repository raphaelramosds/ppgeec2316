# Fast Marching Method (FMM): explicação didática

Este texto explica a ideia do método de forma geral, alinhada ao que aparece em `fmm.py`, sem depender de uma linguagem específica. A intenção é mostrar o raciocínio do algoritmo e como ele se relaciona com a equação de Eikonal e com o código do arquivo.

## 1. A ideia central

O Fast Marching Method resolve problemas em que queremos saber o tempo de chegada de uma onda a cada ponto de um domínio.

Imagine uma fonte emitindo uma frente de onda. A onda se propaga em todas as direções, e cada ponto da malha recebe um valor de tempo de chegada:

- a fonte começa com tempo 0;
- pontos mais próximos da fonte recebem tempos pequenos;
- pontos mais distantes recebem tempos maiores;
- a propagação respeita a velocidade local do meio.

Em termos matemáticos, isso está ligado à equação de Eikonal:

\|∇u(x)\| = 1 / v(x)

onde:

- u(x) é o tempo de chegada em cada ponto;
- v(x) é a velocidade local de propagação;
- a ideia é que a onda se move mais rápido em regiões de maior velocidade.

No código, isso aparece de forma discreta: cada célula da malha guarda um valor `val` que representa o tempo estimado de chegada.

## 2. Como o código modela a malha

No arquivo, cada ponto da grade é representado por um objeto `Node`:

- `label`: define em qual estado a célula está;
- `val`: valor do tempo de chegada.

Os estados são:

- `ACCEPTED`: a célula já foi “fixada” como parte da solução;
- `CONSIDERED`: a célula potencialmente pode entrar na solução;
- `FAR`: ainda não foi alcançada pela onda.

A estrutura principal é uma grade bidimensional `n x n`.

A fonte é colocada em uma posição `(sx, sy)`, e a célula correspondente recebe:

- `val = 0.0`
- `label = ACCEPTED`

Isso significa: o tempo de chegada na fonte é zero.

## 3. O papel da velocidade

No código, a velocidade é assumida constante:

- `f = [[0.3 for _ in range(n)] for _ in range(n)]`

Ou seja, a onda se move com velocidade 0.3 em toda a malha. Esse é um caso simples e útil para introduzir o método. Em problemas mais gerais, a velocidade pode variar de ponto para ponto, mas a lógica do algoritmo continua a mesma.

No FMM, a velocidade influi diretamente na atualização dos valores: mais velocidade implica menor tempo de chegada e, portanto, menor custo para alcançar uma célula.

## 4. A atualização local da célula

A função chamada `eikonal_solver` é o coração do método.

Ela recebe:

- a malha;
- a posição da célula `(i, j)`;
- a velocidade local `v`;
- o tamanho do grid.

O objetivo é determinar o novo valor de tempo de chegada da célula com base nos vizinhos já aceitos.

### 4.1. Menor tempo em direção horizontal

O código calcula `ux` olhando para os vizinhos na horizontal:

- à esquerda, `i-1`;
- à direita, `i+1`.

Se um vizinho for `ACCEPTED`, o algoritmo considera o menor tempo entre eles:

- `ux = min(ux, valor_vizinho)`

O mesmo é feito verticalmente para obter `uy`.

Essas variáveis representam a melhor contribuição já conhecida em cada direção.

### 4.2. Solução 1D

O código então cria um valor de referência:

```python
u_1d = min(ux + h / v, uy + h / v)
```

Essa expressão é a ideia de “um passo de propagação” em uma direção. O termo `h / v` é o tempo necessário para percorrer um passo de malha de tamanho `h` com velocidade `v`.

Em outras palavras:

- se a onda chega de um lado ou de outro;
- o tempo local é aproximadamente o melhor valor conhecido mais o tempo de atravessar uma aresta.

### 4.3. Solução 2D

Quando ambos os lados (`ux` e `uy`) existem, o código tenta resolver a versão bidimensional do problema.

A condição de existência é:

```python
abs(ux - uy) < h / v
```

Se esse critério for atendido, o código calcula um discriminante e então a solução 2D:

```python
u_2d = 0.5 * (ux + uy) + 0.5 * sqrt(delta)
```

Esse passo é importante porque a onda não precisa se mover em apenas uma direção; ela pode vir de vários lados ao mesmo tempo. O método resolve, portanto, uma aproximação local da equação de Eikonal considerando as contribuições dos vizinhos relevantes.

Se as condições não forem favoráveis, ele usa o caso 1D como fallback.

## 5. Como os vizinhos são atualizados

A função `atualizar_vizinhos` percorre os quatro vizinhos mais próximos de uma célula:

- cima;
- baixo;
- esquerda;
- direita.

Para cada vizinho válido dentro da malha:

- se ele ainda não foi aceito;
- calcula-se um novo valor de chegada com `eikonal_solver`;
- se o novo valor for menor que o valor já existente, ele é atualizado;
- o estado passa para `CONSIDERED`.

Isso significa: uma célula ainda não aceita pode ser revisitada várias vezes até que o melhor valor de propagação seja encontrado.

## 6. O coração do algoritmo: a seleção do menor valor

A função `fmm` organiza a propagação da frente de onda.

### Etapa 1: inicialização

- cria a malha com todos os valores infinitos;
- coloca a fonte em `val = 0` e `label = ACCEPTED`;
- atualiza os vizinhos da fonte.

### Etapa 2: loop principal

O código procura, entre todas as células `CONSIDERED`, a que tem o menor tempo de chegada.

Essa é a chave do Fast Marching Method: a onda avança sempre pela célula com menor tempo de chegada, preservando a ordem temporal de propagação.

Quando a menor célula é encontrada:

- ela passa para `ACCEPTED`;
- seus vizinhos são atualizados;
- o processo continua até que não existam mais células `CONSIDERED`.

Esse comportamento garante que, ao aceitar uma célula, o algoritmo já conhece um valor suficientemente bom para ela, porque a ordem de propagação respeita o sentido temporal da onda.

## 7. Por que esse método se chama “Fast Marching”

O nome vem da ideia de uma frente de onda avançando e “marchando” pelo domínio da malha.

A lógica é:

1. a fonte começa a partir de um ponto;
2. a onda se expande;
3. sempre que a frente encontra um ponto ainda não resolvido, ele é atualizado;
4. o próximo ponto a ser resolvido é sempre o mais barato em termos de tempo de chegada.

Isso cria uma marcha ordenada, em que o algoritmo resolve os pontos conforme a ordem natural da propagação da onda.

## 8. Relação com a equação de Eikonal

O FMM é uma ferramenta muito útil para resolver equações do tipo:

\|∇u\| = 1 / v

em geometria e problemas de propagação, por exemplo:

- ondas acústicas;
- ondas sísmicas;
- distância geodésica em superfícies;
- modelos de propagação em imagens e mapas.

No contexto do código, o valor `u_grid[i][j]` representa o tempo de primeira chegada na célula `(i, j)`. Isso é exatamente o que a equação eikonal descreve em sua forma discreta.

## 9. Observações importantes sobre o código

Há algumas características relevantes do arquivo `fmm.py`:

- a velocidade é constante, o que simplifica a exemplificação;
- o grid é quadrado (`n x n`);
- a solução usa os vizinhos diretos apenas (4-neighborhood);
- a ordem do processamento depende do menor valor `CONSIDERED`;
- o algoritmo termina quando não há mais células em estado `CONSIDERED`.

Além disso, vale notar que o código é didático, não necessariamente o mais eficiente do mundo. O comentário no loop mostra uma observação importante:

- a busca do menor valor entre todos os `CONSIDERED` é custosa;
- em uma implementação mais sofisticada, isso costuma ser feito com uma fila de prioridade.

Essa otimização é comum em implementações profissionais do FMM, mas a ideia fundamental continua a mesma.

## 10. Resumo em uma frase

O Fast Marching Method resolve a propagação de uma onda em uma malha ao avançar sempre pela célula com menor tempo de chegada, usando vizinhos aceitos para atualizar a solução local e respeitando a equação de Eikonal.

Em termos práticos, no código:

- a fonte começa com tempo zero;
- o algoritmo identifica o próximo ponto mais “barato” para ser aceito;
- cada novo ponto atualizado influencia os vizinhos;
- a malha inteira é preenchida com tempos de chegada até que a frente de onda termine de se propagar.

Esse é o espírito do FMM: uma onda “marchando” no domínio conforme o tempo da propagação.
