#!/usr/bin/env python3
# -*- coding: utf-8 -*-

from random import randint

M = 10**500
m = 2147483647
with open('inttest.in', 'w') as f1:
    with open('inttest.out', 'w') as f2:
        for i in range(100):
            a = randint(0, M)
            b = randint(0, m)
            f1.write('%d / %d\n' % (a, b))
            f2.write('%d, %d' % divmod(a, b) + '\n')
