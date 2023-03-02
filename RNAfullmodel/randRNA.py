#!/usr/bin/python3

'''
Make a poster of very many randomly generated sequences and their secondary structure
takes file folded_output.txt and makes poster
'''

import sys,math,os,string,random,subprocess

#from PIL import Image
# import matplotlib as mpl
# import matplotlib.pyplot as plt
# import matplotlib.colors as colors
# import matplotlib.cm as cm
# from matplotlib.lines import Line2D
# import matplotlib.gridspec as gridspec
import numpy as np
COUNTERPOS = 0
STRPOS =1


d1={}

# d1 is a dictionary that works like this:
# d1={sha1:[counter,{str1:[counter,[seq1]],str2}] ,sha2,...}

input_filename = "folded_output.txt"
with open(input_filename,"r") as fin:
    for line in fin.readlines():
        seq,str,sha=line.split(' ')
        sha=sha[:-1] #there's a '\n' character
        # if we have seen this shapiro str
        if sha in d1:
            d1[sha][COUNTERPOS] += 1
            # if we have seen this dots-and-br str
            if str in d1[sha][STRPOS]:
                d1[sha][STRPOS][str][COUNTERPOS]+=1
                #print(d1[sha][STRPOS][str][STRPOS])
                d1[sha][STRPOS][str][STRPOS].append(seq)
            else:
                d1[sha][STRPOS][str]=[1,[seq]]
        else:
            d1[sha]=[1,{str:[1,[seq]]}]
        #     if str in d1[sha][STRPOS]:
        #         d1[sha][STRPOS][str][COUNTERPOS]+=1
        #         if seq in d1[sha][STRPOS][str][STRPOS]:
        #             print("Weird, got duplicated sequence, doesn't matter, we go on")
        #         d1[sha][STRPOS][str][STRPOS].append(seq)
        #     else:
        #         d1[sha][STRPOS][str]=[1,[seq]]
        # else:
        #     d1[sha]=[1,]

#SORTING d1:
#l1 = [(k, d[k]) for k in sorted(d1, key=d1.get, reverse=True)]
# x = {1: [2,10], 3: [4,12], 4: [3,13], 2: [1,11], 0: [0,15]}
# sorted_x = sorted(x.items(), key=lambda kv: kv[1][1])
# print(x)
# print(sorted_x)
# sys.exit(1)

#sorted() makes a list, sorted by the values of d1 (the .items() method returns a list of tuples [(key,val),...]) and those are sorted on the basis of value[COUNTERPOS] via the lambda function
# the list is a list of tuples [(key,value),(key,val),...]

#sorted_d1 = sorted(d1.items(), key=lambda kv: kv[COUNTERPOS])
#maybe I should just get a list of sha, and get them in order according to their abundances
sorted_d1 = sorted(d1.items(), key=lambda kv: kv[1][0])
for sha,val_sha in sorted_d1:
    print(sha,val_sha[COUNTERPOS])
    sorted_val = sorted(val_sha[STRPOS].items(), key=lambda kv: kv[1][0])
    for str,val_str in sorted_val:
        print('\t', str, val_str[COUNTERPOS])
    print()
    
#also prints a recap
print("#recap: ")
for sha,val_sha in sorted_d1:
    print(sha,val_sha[COUNTERPOS])

sys.exit(1)

for sha in d1:
    print(sha, d1[sha][COUNTERPOS])
    for str in d1[sha][STRPOS]:
        print('\t',str, d1[sha][STRPOS][str][COUNTERPOS])
    print()


sys.exit(1)

Nseq=100000
lenseq=80
seq_output_filename = "seq_output.txt"

lseq=[]
for i in range(Nseq):
  lseq.append(  ''.join(random.choices(['A','U','C','G'], k=lenseq))  )
  #if i%100000==0: print(i)

print("Done generating sequences")
i=0
with open(seq_output_filename,'w') as fout:
    for x in lseq: 
        #fout.write('>'+str(i)+'\n')
        fout.write(x+'\n')
        i+=1
print("Sequence saved to file: ", seq_output_filename)
print("Now folding, this may take a while")

#subprocess.run(['ls', '-l'], stdout=subprocess.PIPE)
#RNAfold --noPS -j --unordered --infile=seq_output.txt

result = subprocess.run(['RNAfold', '--noPS', '-j', '--unordered', '--infile='+seq_output_filename], stdout=subprocess.PIPE)
print("Done folding, output:")
folded_output = result.stdout.decode('utf-8') # notice that output is a bytes object, we have to decode this into string 
print(folded_output) 

#lfolded_output = folded_output.split('\n')
#print(lfolded_output)

print("The end")