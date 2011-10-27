import JFLAPParser
import xml.sax
import pprint
import Model


parser = xml.sax.make_parser()
handler = JFLAPParser.StateHandler()
parser.setContentHandler(handler)
parser.parse("Input3.jff")
#pprint.pprint(handler.states)

systemModel = Model.DFA()

for stateId in handler.states:
    node = Model.DFAState()
    parsedNode = handler.states[stateId]
    node.Id = parsedNode.Id
    node.Name = parsedNode.Name
    for transition in parsedNode.Transitions:
        node.Transitions[transition[0]] = transition[1]
    systemModel.addState(node)

systemModel.connectGraph()

print systemModel.graph[0]
print systemModel.graph[1]
print systemModel.graph[2]

        
