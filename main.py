from InputFile import StateHandler
import xml.sax
import pprint

parser = xml.sax.make_parser()
handler = StateHandler()
parser.setContentHandler(handler)
parser.parse("Input1.jff")
pprint.pprint(handler.states)

for stateId in handler.states:
    handler.states[stateId].ToString()
