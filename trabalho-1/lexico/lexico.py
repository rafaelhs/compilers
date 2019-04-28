from lib.hashtable import Hashtable
from lib.automata import Automata
from lib.token import Token





def is_reserved(reserved_list, identifier):
    for reserved_word in reserved_list:
        if reserved_word == identifier:
            return True
    return False


r_in = open("reserved_words.txt", "r") # Arquivo com a lista de palavras reservadas
records_out = open("records.txt", "w")  # Arquivo com a lista de records
hash_out = open("hashtable.txt", "w")  # Arquivo com a hashtable de identificadores
f_in = open("in.txt", "r")
reserved_words = [line.rstrip('\n') for line in r_in]


hash = Hashtable(97)  # Hashtable
at = Automata(14)  # Automato
records = []  # Lista de records

#Iniciando as tansicoes do automato
for i in range(ord('a'), ord('z')+1):
    at.put(0, i, 1)
    at.put(1, i, 1)

for i in range(ord('A'), ord('Z')+1):
    at.put(0, i, 1)
    at.put(1, i, 1)

for i in range(ord('0'), ord("9")+1):
    at.put(1, i, 1)
    at.put(0, i, 3)
    at.put(2, i, 3)
    at.put(3, i, 3)
    at.put(13, i, 4)
    at.put(4, i, 4)

at.put(3, ord('.'), 13)
at.put(0, ord('-'), 2)
at.put(0, ord('+'), 2)

at.put(0, ord(')'), 5)
at.put(0, ord('_'), 5)
at.put(0, ord('\''), 5)
at.put(0, ord('}'), 5)
at.put(0, ord('{'), 5)
at.put(0, ord(']'), 5)
at.put(0, ord('['), 5)
at.put(0, ord('='), 5)
at.put(0, ord(';'), 5)

at.put(0, ord(':'), 6)
at.put(0, ord('('), 7)
at.put(0, ord('*'), 8)
at.put(0, ord('<'), 9)
at.put(0, ord('>'), 10)
at.put(0, ord('.'), 11)

at.put(11, ord('.'), 12)
at.put(10, ord('='), 12)
at.put(9, ord('='), 12)
at.put(8, ord(')'), 12)
at.put(7, ord('*'), 12)
at.put(6, ord('='), 12)


#Analise lexica
row_count = 0
col_count = 0
error_flag = False
for line in f_in.readlines():
    if error_flag:
        break

    col_count = 0
    row_count += 1
    text = line

    string_start = 0
    current_state = 0
    next_state = 0
    last_final = 0
    last_final_state = 0

    i = 0
    l = len(text)

    while l > 0:
        if error_flag:
            break

        ch = text[i]
        # Verificando o proximo estado
        next_state = at.get(current_state, ord(ch))
        if not(next_state == None):  # Proximo estado existe
            current_state = next_state  # Anda para o proximos estado
            if(at.is_final(current_state)):  # Se o estado for final, guarda a informacao
                last_final = i  # Ultimo caracter final
                last_final_state = current_state  # Ultimo estado final
            i += 1
        else:  # movimento nao valido
            if not(last_final_state == 0):  # Um estado final valido tinha sido encontrado
                token_value = text[0:last_final+1]  # Valor da token
                text = text[last_final+1:]  # Retirando o valor do texto
                token_type = at.get_state(last_final_state)
                if token_type == "IDENTIFIER":  # Se for um identificador, verifica se e palavra reservada
                    # Verificando se e palavra reservada
                    if(is_reserved(reserved_words, token_value.lower())):
                        token_type = "RESERVED_WORD"  # Mudando tipo da token para palavra reservada
                token = Token(token_value, token_type)
                records.append(token)  # Adicionando token na lista de records
                if token.get_type() == "IDENTIFIER":  # Se for identificador, insere na hash
                    hash.insert(token_value, token)

            else:  # Nenhum estado final enconrtado: erro
                if not(ch == " ") and not(ch == "\n"):  # Ignora espacos vazios e quebras de linha
                    print("Linha: ", row_count, " Coluna: ", col_count+1, sep="")
                    print("Caracter desconhecido \"", text[i], "\"", sep="")
                    error_flag = True
                text = text[1:]  # deleta caracter
                i += 1
            col_count += i
            i = 0
            l = len(text)
            current_state = 0
            next_state = 0
            last_final = 0
            last_final_state = 0

        if i >= l:
            if not(last_final_state == 0):  # Um estado final valido tinha sido encontrado
                token_value = text[0:last_final+1]  # Valor da token
                text = text[last_final+1:]  # Retirando o valor do texto
                token_type = at.get_state(last_final_state)
                if token_type == "IDENTIFIER":  # Se for um identificador, verifica se e palavra reservada
                    # Verificando se e palavra reservada
                    if(is_reserved(reserved_words, token_value.lower())):
                        token_type = "RESERVED_WORD"  # Mudando tipo da token para palavra reservada
                token = Token(token_value, token_type)
                records.append(token)  # Adicionando token na lista de records
                if token.get_type() == "IDENTIFIER":  # Se for identificador, insere na hash
                    hash.insert(token_value, token)

            else:  # Nenhum estado final enconrtado: erroo
                if not(ch == " ") and not(ch == "\n"):
                    print("Linha: ", row_count, " Coluna: ", col_count+1, sep="")
                    print("Caracter desconhecido \"", text[i], "\"", sep="")
                    error_flag = True
                text = text[1:]  # deleta caracter

            col_count += i
            i = 0
            l = len(text)
            current_state = 0
            next_state = 0
            last_final = 0
            last_final_state = 0


if not(error_flag):
    print("\n---------------------------------")
    print("\nLista de records: \n")
    for x in records:
        write_str = x.get_value() + " - " + x.get_type()
        print(write_str)
        records_out.write(write_str + '\n')


    print("\n---------------------------------")
    print("\nTabela hash: \n")
    for l in hash.get_pos():
        write_str = "Position: " + str(l[0]) + " "
        print(write_str)
        hash_out.write(write_str + "\n")

        for tk in l[1]:
            write_str = tk[1].get_value() + " - " + tk[1].get_type()
            print(write_str)
            hash_out.write(write_str + "\n")

        print("\n")
        hash_out.write("\n")

