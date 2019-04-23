from lib.hashtable import Hashtable
from lib.automata import Automata
from lib.token import Token


def hash_function(key: str, mod: int):
    x = 1
    sum = 0
    for char in key:
        sum += ord(char) * x
        x += 1
    return sum % mod


def verify_reserved(reserved_list, identifier):
    for reserved_word in reserved_list:
        if reserved_word == identifier:
            return True
    return False


r_in = open("reserved_words.txt", "r")
f_out = open("out.txt", "w")
f_in = open("in.txt", "r")
reserved_words = [line.rstrip('\n') for line in r_in]


hash = Hashtable(111, hash_function)  # Hashtable
at = Automata(14)  # Automato
records = []  # Lista de records
if pauNaTuaBunda == true:
    motel += 1


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

'''
text = f_in.read()

string_start = 0
current_state = 0
next_state = 0
last_final = 0
last_final_state = 0

i = 0
l = len(text)
'''

for line in f_in.readlines():
    print(line)
    text = line

    string_start = 0
    current_state = 0
    next_state = 0
    last_final = 0
    last_final_state = 0

    i = 0
    l = len(text)

    while l > 0:
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
                text = text[last_final+1:]

                token = Token(token_value, at.get_state(last_final _state))
                hash.insert(token_value, token)
                # f_out.write(token + " - " + at.get_state(last_final_state) + "\n")

            else:  # Nenhum estado final enconrtado: erroo
                if not(ch == " ") and not(ch == "\n"):
                    print("Caracter desconhecido \"", text[i], "\"", sep="")
                text = text[1:]  # deleta caracter

            i = 0
            l = len(text)
            current_state = 0
            next_state = 0
            last_final = 0
            last_final_state = 0

        if i >= l:
            if not(last_final_state == 0):  # Um estado final valido tinha sido encontrado

                token = text[0:last_final+1]
                text = text[last_final+1:]
                f_out.write(token + " - " +
                            at.get_state(last_final_state) + "\n")

            else:  # Nenhum estado final enconrtado: erroo
                if not(ch == " ") and not(ch == "\n"):
                    print("Caracter desconhecido \"", text[i], "\"", sep="")
                text = text[1:]  # deleta caracter

            i = 0
            l = len(text)
            current_state = 0
            next_state = 0
            last_final = 0
            last_final_state = 0
