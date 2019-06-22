import numpy as np
import netCDF4
from netCDF4 import Dataset
import xarray
from scipy import signal
from scipy.ndimage.filters import convolve
from scipy.misc import imread, imshow
import matplotlib.pyplot as plt
import matplotlib.image as mpimg
from matplotlib.colors import LinearSegmentedColormap

'''
Parametros
'''
w = np.array([[-1,-1,-1],[-1,8,-1],[-1,-1,-1]]) #MATRIZ CON LA QUE SE REALIZA LA CONVOLUCION

dataDIR = "./src/OR_ABI_Convolution.nc"

'''
Abrir el dataset como una matriz XARRAY y guardar la matriz CMI
'''
DS = Dataset(dataDIR)
f = DS.variables['CMI']
#print f.shape
#print f

h = f[0:21696,0:21696]

plt.imshow(h[0:5000,0:5000],cmap=plt.cm.get_cmap('hot',2))
plt.savefig('original.png',format="png",dpi=1200)
#plt.show()