class DFA(object):
    def __init__(self):
        self.head = None
        self.graph = {}

    def addState(self, state):
        if state.IsInital:
            self.head = state
        self.graph[state.Id] = state

    def connectGraph(self):
        for node in self.graph.values():
            for label in node.Transitions:
                node.Transitions[label] = self.graph[node.Transitions[label]]

    def neighbors(self, node):
        return node.Transitions.values() # Return the nodes

    # Code adapted from http://code.activestate.com/recipes/576946-depth-first-search-generator/
    def DFS(self, v):
        yield v
        visited = set([v])
        S = self.neighbors(v)
        while S:
            w = S.pop()
            if w not in visited:
                yield w
                visited.add(w)
                S.extend(self.neighbors(w))

    def __str__(self):
        string = ""
        for Node in self.DFS(self.head):
            string = string + str(Node)
        return string
        

class DFAState(object):
    def __init__(self):
        self.Id = 0
        self.Name = ""
        self.Transitions = {}
        self.IsInital = False
        self.IsFinal = False
    
    def addTransition(self, label, toState):
        self.Transitions[label] = toState

    def __str__(self):
        return str(self.Name)+str(self.Transitions)
