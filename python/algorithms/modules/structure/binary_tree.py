from random import randint

class Node:
    def __init__(self, key = None, value = None, left = None, right = None):
        self.key = key
        self.value = value
        self.children = {0: left, 1:right}



class BinaryTree:

    def __init__(self):
        self._root = None
        self._hight = 0
        self._indent_size = 4

    def put(self, key, value):
        if(self._root == None):
            self._root = Node(key, value)

            return

        node = self._root
        parent = None
        child_right = None
        while True:
            if(node == None):
                parent.children[child_right] = Node(key, value)
                break
            elif(node.key == key):
                node.value = value
                break
            else:
                parent = node
                child_right = parent.key < key
                node = parent.children.get(child_right)


    def remove(self, key):
        node = self._root
        parent = None
        while node != None:
            if(node.key == key):
                if all(value == None  for value in node.children.values()):
                    parent.children[parent.key < key] = None
                    break
                elif all(value != None for value in node.children.values()):
                    max_lower_node = node.children.get(0)
                    max_lower_parent = node
                    children_left = 0
                    while max_lower_node.children.get(1) != None:
                        max_lower_parent = max_lower_node
                        max_lower_node = max_lower_node.children.get(1)
                        children_left = 1

                    node.key = max_lower_node.key
                    node.value = max_lower_node.value
                    max_lower_parent.children[1] = max_lower_node.children.get(children_left)
                else:
                    parent.children[parent.key < key] = node.children[node.children.get(0) == None]
                break
            else:
                parent = node
                node = parent.children.get(parent.key < key)

    def get(self, key):
        node = self._root
        while True:
            if(node == None):
                return None
            elif(node.key == key):
                return node.value
            else:
                node = node.children[node.key < key]

    def check_tree(self):
        self._symmetric_walk(self._root, self._confirm_node)

    def describe(self):
        if (self._root == None):
            print("Tree is empty")
            return

        print()
        print("horizontal:")
        self._horizontal_walk(self._print_node)
        print()
        print("preorder:")
        self._preorder_walk(self._root, self._print_node)
        print()
        print("symmetric:")
        self._symmetric_walk(self._root, self._print_node)
        print()
        print("postorder:")
        self._postorder_walk(self._root, self._print_node)

    def _horizontal_walk(self, accept_func):
        queue = [self._root]
        while len(queue) > 0:
            node = queue.pop()
            accept_func(node)
            if(node.children.get(0) != None):
                queue.insert(0, node.children.get(0))
            if (node.children.get(1) != None):
                queue.insert(0, node.children.get(1))


    def _preorder_walk(self, node, accept_func):
        if (node != None):
            accept_func(node)
            self._preorder_walk(node.children.get(0), accept_func)
            self._preorder_walk(node.children.get(1), accept_func)

    def _symmetric_walk(self, node, accept_func):
        if (node != None):
            self._symmetric_walk(node.children.get(0), accept_func)
            accept_func(node)
            self._symmetric_walk(node.children.get(1), accept_func)

    def _postorder_walk(self, node, accept_func):
        if (node != None):
            self._postorder_walk(node.children.get(0), accept_func)
            self._postorder_walk(node.children.get(1), accept_func)
            accept_func(node)

    def _print_node(self, node):
        print("{", node.key, ":", node.value, "},", sep="", end="")

    def _confirm_node(self, node):
        if(node.children.get(0) != None and node.children.get(0).key >= node.key):
            print("Tree check fail on left. parent=", node.key,"; children=", node.children.get(0).key)
        if (node.children.get(1) != None and node.children.get(1).key <= node.key):
            print("Tree check fail on right. parent=", node.key,"; children=", node.children.get(1).key)

def test_tree():
    test_count = 200
    item_count = 10
    key_entrophy = 200000
    for i in range(0, test_count):
        tree = BinaryTree()
        to_delete = None
        for i in range(0, item_count):
            key = randint(0, key_entrophy)
            print(key, end=" ")
            if (i == round(item_count / 2)):
                to_delete = key
            value = chr(randint(140, 250))
            tree.put(key, value)
        print()
        tree.check_tree()
        tree.remove(to_delete)
        print("-----------------")
        tree.check_tree()
        print("=================")


test_tree()

