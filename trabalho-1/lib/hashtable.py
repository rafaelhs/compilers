class Hashtable:
    __max = None  # Tamanho da tabela
    __hash_function = None  # Funcao de hash
    __size = None  # Quantos itens estao atualmente na tabela
    __table = None  # Tabela com a lista de itens

    def __init__(self, max, hash_function):
        self.__max = max
        self.__hash_function = hash_function
        self.__size = 0
        self.__table = [None] * self.__max

    # Insere o item na tabela baseado na chave passada
    def insert(self, key, item):
        node = Node(key, item)
        max = self.__max
        hash_function = self.__hash_function

        # Se a lista nao existir, inicia uma lista vazia
        lst = self.__table[hash_function(key, max)]
        if lst == None:
            lst = []

        lst.append(node)
        self.__table[hash_function(key, max)] = lst
        self.__size += 1

    # Retorna o item com a chave dada, retorna None se nao encontrar
    def get(self, key):
        max = self.__max
        hash_function = self.__hash_function
        lst = self.__table[hash_function(key, max)]

        # Se lista nao existe retorna None
        if(lst == None):
            return None

        # Tenta encontrar o item com a chave na lista
        for n in lst:
            if n.get_key() == key:
                return n.get_item()

        return None

    def get_max(self):
        return self.__max

    def get_hash_function(self):
        return self.__hash_function

    def get_size(self):
        return self.__size

    def get_table(self):
        return self.__table


#Tupla (chave/item)
class Node:
    __key = None
    __item = None

    def __init__(self, key, item):
        self.__key = key
        self.__item = item

    def get_item(self):
        return self.__item

    def get_key(self):
        return self.__key
