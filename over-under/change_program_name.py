import json
import sys

assert len(sys.argv) == 2

with open("project.pros") as f: 
    s=f.read()

d = json.loads(s)

d['py/state']['project_name'] = sys.argv[1]

with open("project.pros", "w") as f:
    f.write(json.dumps(d))