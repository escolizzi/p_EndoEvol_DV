#!/usr/bin/python3

'''
Make a poster of very many randomly generated sequences and their secondary structure
takes file folded_output.txt and makes poster
'''

import sys,math,os,string,random,subprocess

#from PIL import Image
import matplotlib as mpl
import matplotlib.pyplot as plt
import matplotlib.colors as colors
import matplotlib.cm as cm
from matplotlib.lines import Line2D
import matplotlib.gridspec as gridspec
import numpy as np
COUNTERPOS = 0
STRPOS =1


l1=[]

# d1 is a dictionary that works like this:
# d1={sha1:[counter,{str1:[counter,[seq1]],str2}] ,sha2,...}
begin_found=False

input_filename = "RNAposter.seq"
with open(input_filename,"r") as fin:
    for line in fin.readlines():
        if begin_found==False:
            if line[0]!='#': 
                continue
            if line[0]=='#':
                begin_found=True
                continue
        sha,num = line.split(' ')
        #print (sha,num)
        l1.append( math.log10(float(num)))
        # l1.append( int(num))
        
print("Done reading")
# bins = np.arange(0, max(l1) + 1.5) - 0.5
fig, ax = plt.subplots()
# ax.hist(l1, bins=100)
ax.hist(l1, bins=100, log=True)
#ax.hist(l1,log=True)
ax.set_ylabel('# of seq folding into a certain sha_str')
ax.set_xlabel('abundance')

plt.show()