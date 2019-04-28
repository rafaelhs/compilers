
class Automata():

    def __init__(self, states):

        self.automata = []
        for _ in range(states+1):
            col = []
            for _ in range(255):
                col.append(None)
            self.automata.append(col)

    def put(self, current_state, char, next_state):
        self.automata[current_state][char] = next_state

    def get(self, current_state, char):
        return self.automata[current_state][char]

    def is_final(self, state):
        if state in [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12]:
            return True
        else:
            return False

    def get_state(self, state):
        if state in [4, 3]:
            return 'NUMBER'
        elif state in [1]:
            return 'IDENTIFIER'
        elif state in [2, 5, 6, 7, 8, 9, 10, 11]:
            return 'SYMBOL'
        elif state in [12]:
            return 'COMPOSITE_SYMBOL'
        else:
            return 'ERROR'
