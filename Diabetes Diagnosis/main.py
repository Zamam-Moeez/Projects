import pandas as pd
import numpy as np
import sklearn as sk
import tensorflow as tf

def convert_dtype(x):
    if not x:
        return ''
    try:
        return str(x)
    except:
        return ''

dataf = pd.read_csv('diabetes.csv',converters={'first_column': convert_dtype,'second_column': convert_dtype})

inspecion = dataf.describe()

ds = tf.data.Dataset.from_tensor_slices(dict(dataf))

print(ds)

for elem in ds:
    print(elem)

























