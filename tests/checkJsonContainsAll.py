#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Thu May  9 09:18:14 2019

@author: grat05
"""

import PyLongQt as pylqt
import json
import re

"""
simvars descriptions
"""
print('Simvars: ',end='')

filename = './hoverText/simvarsDesc.json'
with open(filename) as f:
    data = json.load(f)

missingList = []
for protocol in pylqt.protoMap:
    proto = pylqt.protoMap[protocol]()
    parNames = [p[0] for p in proto.pars]
    for name in parNames:
        if name not in data:
            missingList.append(name)
print(len(missingList) == 0)
print(missingList)

"""
measure descriptions
"""
print('Measures: ',end='')
measureTypes = ['Default', 'Measure Voltage']

filename = './hoverText/measDesc.json'
with open(filename) as f:
    data = json.load(f)


missingList = []
for meas in measureTypes:
    measOpts = pylqt.Misc._MeasureFactory().measureOptions(meas)
    for name in measOpts:
        if name not in data:
            missingList.append(name)
print(len(missingList) == 0)
print(missingList)

"""
Cell Pars
"""
print('Cell Pars: ',end='')

filename = './hoverText/cellParsDesc.json'
with open(filename) as f:
    data = json.load(f)


missingList = []
for cellType in pylqt.cellMap:
    cell = pylqt.cellMap[cellType]()
    for name in cell.pars:
        if name not in data and re.search('factor', name, re.IGNORECASE) and not re.search('test', name, re.IGNORECASE):
            missingList.append(name)
print(len(missingList) == 0)
print(missingList)

"""
Cell Vars
"""
print('Cell Vars: ',end='')

filename = './hoverText/cellVarsDesc.json'
with open(filename) as f:
    data = json.load(f)


missingList = []
for cellType in pylqt.cellMap:
    cell = pylqt.cellMap[cellType]()
    for name in cell.vars:
        if name not in data:
            missingList.append(name)
print(len(missingList) == 0)
print(missingList)
