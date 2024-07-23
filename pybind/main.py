import sys

sys.path.append(".build")

import mylib, pyarrow as pa

tbl = mylib.test_get()
print(tbl)
# print(tbl.to_pandas())
