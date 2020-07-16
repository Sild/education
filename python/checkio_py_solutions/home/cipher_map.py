def recall_password(cipher_grille, ciphered_password):
    password = ""
    for _i in range(0, 4):
        password += apply(ciphered_password, cipher_grille, "X")
        cipher_grille = transform(cipher_grille)
    return password


def apply(password_matrix, cipher_matrix, char):
    part = ""
    for line in zip(cipher_matrix, password_matrix):
        for first, second in zip(*line):
            if first == char:
                part += second
    return part


def transform(matrix):
    return [line[::-1] for line in zip(*matrix)]


if __name__ == '__main__':
    #These "asserts" using only for self-checking and not necessary for auto-testing
    assert recall_password(
        ('X...',
         '..X.',
         'X..X',
         '....'),
        ('itdf',
         'gdce',
         'aton',
         'qrdi')) == 'icantforgetiddqd', 'First example'

    assert recall_password(
        ('....',
         'X..X',
         '.X..',
         '...X'),
        ('xhwc',
         'rsqx',
         'xqzz',
         'fyzr')) == 'rxqrwsfzxqxzhczy', 'Second example'

