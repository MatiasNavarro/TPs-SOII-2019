#!/bin/python
import numpy as np
import netCDF4
from netCDF4 import Dataset
import xarray
from scipy import signal
from scipy.ndimage.filters import convolve
from scipy.misc import imread, imshow
import matplotlib.pyplot as plt

'''
Parametros
'''
w = np.array([[-1,-1,-1],[-1,8,-1],[-1,-1,-1]]) #MATRIZ CON LA QUE SE REALIZA LA CONVOLUCION
print w

dataDIR = "/home/matiasnavarro/Facultad/2019/Sistemas_Operativos_II/Practicos/TP2-Recursos/OR_ABI.nc"

'''
Abrir el dataset como una matriz XARRAY y guardar la matriz CMI
'''
DS = Dataset(dataDIR)
f = DS.variables['CMI']
print f.shape

h = f[10000:15000,10000:15000]
print h.shape
print h
plt.imshow(h)
plt.show()
'''
Arreglo = np.array([[10,2,2,2,2],[2,10,2,2,2],[2,2,10,2,2],[2,2,2,10,2],[2,2,2,2,10]])
print (Arreglo)

Convolucion

g = signal.convolve2d(Arreglo,w,boundary='fill',mode='same')
h = convolve(Arreglo,w)
y = signal.convolve2d(Arreglo,w,boundary='symm',mode='valid')
print(g)
print g.shape
print(h)
print h.shape
print(y)
print y.shape

y = g[10000:10200,10000:10200]
print y.shape
plt.imshow(y)
plt.show()'''
