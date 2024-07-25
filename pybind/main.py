import sys
from typing import cast
from pandas import DataFrame

sys.path.append(".build")

import window_cpp


tbl = window_cpp.run()
if tbl is None:
    print(tbl)
    exit()
df = cast(DataFrame, tbl.to_pandas())

print(df[["date", "serial_number", "failure"]])
