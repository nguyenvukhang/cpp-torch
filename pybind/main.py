import sys

sys.path.append(".build")

import window_cpp


tbl = window_cpp.run()
if tbl is None:
    print(tbl)
else:
    print(tbl.to_pandas())
