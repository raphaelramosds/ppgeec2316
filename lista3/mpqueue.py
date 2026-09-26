from dataclasses import dataclass

@dataclass
class HeapNode:
    val: float
    heap_index: int = -1

class MinPriorityQueue:

    def __init__(self):
        self.heap: list[HeapNode] = [None]
        self.heap_size: int = 0

    def parent(self, i: int) -> int:
        # retorna indice pai de um node
        return i // 2

    def left(self, i: int) -> int:
        # retorna filho esquerdo de um node
        return 2 * i

    def right(self, i: int) -> int:
        # retorna filho direito de um node
        return 2*i + 1

    def min_heapify(self, i: int) -> None:
        l = self.left(i)
        r = self.right(i)

        # por enquanto assumimos que o menor elemento eh o pai
        menor = i

        # acha qual filho (left ou right) tem menor valor que o pai
        if l <= self.heap_size and self.heap[l].val < self.heap[i].val:
            menor = l

        if r <= self.heap_size and self.heap[r].val < self.heap[menor].val:
            menor = r

        # apenas faca a troca se o pai (i) for maior que um dos dois filhos
        if menor != i:
            # coloque o menor elemento na posicao do pai, mantendo a propriedade da min heap
            self.heap[i], self.heap[menor] = self.heap[menor], self.heap[i]
            self.heap[i].heap_index = i
            self.heap[menor].heap_index = menor
            # corrige a subarvore abaixo 
            # NOTE: observe que o pai agora ocupa a posicao `menor` que era do filho que "subiu" apos a troca ser feita
            self.min_heapify(menor)

    def heap_extract_min(self) -> HeapNode:
        if self.heap_size < 1:
            raise ValueError("[heap] todos os nodes ja foram desenfileirados")

        # acesse o menor elemento da heap, que ja se encontra na raiz (O(1))
        min_node = self.heap[1]

        # substituimos o elemento "removido" pelo ultimo elemento da heap
        self.heap[1] = self.heap[self.heap_size]
        self.heap[1].heap_index = 1
        self.heap_size -= 1

        # removemos a ultima posicao
        self.heap.pop()

        # este elemento ja nao pertence mais a heap
        min_node.heap_index = -1

        if self.heap_size > 0:
            self.min_heapify(1) # comece a corrigir a min-heap pela raiz

        return min_node

    def heap_decrease_key(self, i: int, key: float) -> None:
        if key > self.heap[i].val:
            # chaves menores tem mais prioridade numa min priority queue
            raise ValueError("[priority queue] a nova chave eh maior que a chave atual")
        
        self.heap[i].val = key

        # subida do elemento no heap
        while i > 1 and self.heap[self.parent(i)].val > self.heap[i].val:
            # enquanto o pai for maior que o elemento com chave atualizada
            pai = self.parent(i)

            # troca o elemento com seu pai
            self.heap[i], self.heap[pai] = self.heap[pai], self.heap[i]
            self.heap[i].heap_index = i
            self.heap[pai].heap_index = pai

            # ir para a subarvore acima
            i = pai

    def min_heap_insert(self, node: HeapNode, key: float) -> None:
        self.heap_size += 1
        node.val = float("inf") # maior valor possivel (infinito)
        node.heap_index = self.heap_size
        self.heap.append(node)
        self.heap_decrease_key(self.heap_size, key)

    def is_empty(self) -> bool:
        return self.heap_size == 0