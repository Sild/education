def read_file_as_string(path):
    with open(path, 'r') as file:
        return file.read().replace('\n', '')


def read_file_as_int_array(path):
    data = []
    with open(path, 'r') as file:
        for line in file:
            data.append(int(line))
    return data