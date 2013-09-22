#! /usr/bin/env python
from datetime import datetime
from datetime import time

ts = datetime.strftime(datetime.now(), '+%y%m%d_%H%M%S')
print ts
