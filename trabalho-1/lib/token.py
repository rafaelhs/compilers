class Token:

    def __init__(self, token_value, token_type):
        self.__value = token_value
        self.__type = token_type

    def get_value(self):
        return self.__value

    def get_type(self):
        return self.__type
