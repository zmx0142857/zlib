#!/usr/bin/env bash

./inttest.py
./test < inttest.in > inttest.out2
diff inttest.out inttest.out2
