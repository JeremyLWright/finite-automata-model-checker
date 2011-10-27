#!/usr/bin/python
import xml.sax.handler


class State(object):
    def __init__(self):
        self.Id = 0
        self.IsInital = False
        self.IsFinal = False
        self.Name = ""
        self.Transitions = []

    def ToString(self):
        print("%s:%s(%s/%s)"%(repr(self.Id), self.Name, self.IsInital, self.IsFinal))
        print self.Transitions


class StateHandler(xml.sax.handler.ContentHandler):
    def __init__(self):
        self.i = 0
        self.states = {}
        self.buffer = ""
        self.ParsingFromTransition = False
        self.ParsingToTransition = False
        self.ParsingLabel = False
        self.TmpTransition = {}

    def startElement(self, name, attributes):
        if name == "state":
            self.currentState = State()
            self.currentState.Id = int(attributes["id"])
            self.currentState.Name = attributes["name"]

        elif name == "initial":
            self.currentState.IsInital = True

        elif name == "final":
            self.currentState.IsFinal = True

        elif name == "transition":
            self.TmpTransition = {}

        elif name == "from":
            self.ParsingFromTransition = True

        elif name == "to":
            self.ParsingToTransition = True

        elif name == "read":
            self.ParsingLabel = True
            

    def characters(self, data):
        if self.ParsingLabel:
            self.TmpTransition["label"] = data
        elif self.ParsingFromTransition:
            self.TmpTransition["from"] = int(data)
        elif self.ParsingToTransition:
            self.TmpTransition["to"] = int(data)

    def endElement(self,name):
        if name == "state":
            self.states[self.currentState.Id] = self.currentState

        elif name == "from":
            self.ParsingFromTransition = False

        elif name == "to":
            self.ParsingToTransition = False

        elif name == "read":
            self.ParsingLabel = False
            if not self.TmpTransition.has_key("label"): #Epislon Transition
                self.TmpTransition["label"] = None

        elif name == "transition":
            transitionTuple = self.TmpTransition["label"], self.TmpTransition["to"], 
            self.states[self.TmpTransition["from"]].Transitions.append(transitionTuple)
        


