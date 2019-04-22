
class Automata():

    def __init__(self):

        self.automata = []
        for i in range(29):
            col = []
            for j in range(255):
                col.append(None)
            self.automata.append(col)

    def put(self, current_state, char, next_state):
        self.automata[current_state][char] = next_state

    def get(self, current_state, char):
        return self.automata[current_state][char]

    def is_final(self, state):
        if state in range(1, 29):
            return True
        else:
            return False

    def get_state(self, state):
        if state in [1, 2, 3]:
            return 'NUMBER'
        elif state in [28]:
            return 'IDENTIFIER'
        elif state in range(4, 21):
            return 'SIMPLE_IDENTIFIER'
        elif state in range(21, 27):
            return 'COMPOSITE_IDENTIFIER'
        else:
            return 'ERROR'
