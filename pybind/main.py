import sys

sys.path.append(".build")

import mylib


print(mylib.read_parquet())
