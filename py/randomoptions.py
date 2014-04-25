#! /usr/bin/env python
import random

def choice_opts_sign(opts):
    """diff from choice_opts, opt_name=xx

    Arguments:
    - `opts`:
    """
    return opts[random.randint(0, len(opts)-1)]

options = {}
vers_opts = [ 2,3,4 ]
options["vers"] = (choice_opts_sign, vers_opts, lambda x:x in vers_opts)
print options["vers"][0](options["vers"][1])
