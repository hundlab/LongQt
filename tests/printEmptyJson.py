#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Thu May 23 12:56:39 2019

@author: grat05
"""

import json

filename = './hoverText/cellParsDesc.json'
with open(filename) as f:
    data = json.load(f)

empty = [name for name in data if len(data[name]) == 0 or len(data[name]['Description']) == 0]
print(empty)