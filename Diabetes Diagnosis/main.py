import pandas as pd
import numpy as np
import sklearn as sk
from sklearn.model_selection import train_test_split
import tensorflow as tf

def convert_dtype(x):
    if not x:
        return ''
    try:
        return str(x)
    except:
        return ''
def pre_process():
    dataf = pd.read_csv('diabetes.csv',converters={'first_column': convert_dtype,'second_column': convert_dtype})

    print(dataf.head(3))

    inspecion = dataf.describe()

    print(dataf.columns)

    columns = ["Pregnancies", "Glucose", "BloodPressure", "SkinThickness", "Insulin",
       "BMI", "DiabetesPedigreeFunction", "Age", "Outcome"]

    fet_colm = ["preg_f"]

    datanorm  = dataf.iloc[: , :8] #only need features, drop the Outcome for now

    datanorm=(datanorm-datanorm.min())/(datanorm.max()-datanorm.min())

    print(datanorm)

    inp = datanorm

    oup = df["Outcome"]

    inp_tr, , inp_te, oup_tr, oup_te = train_test_split(inp,oup, test_size=0.2,random_state=101)

    return  inp_tr, , inp_te, oup_tr, oup_te


def features():

    preg_f = tf.feature_column.numeric_column("Pregnancies")

    gluc_f = tf.feature_column.numeric_column("Glucose")

    bl_f = tf.feature_column.numeric_column("BloodPressure")

    sk_f = tf.feature_column.numeric_column("SkinThickness")

    ins_f = tf.feature_column.numeric_column("Insulin")

    bmi_f = tf.feature_column.numeric_column("BMI")

    DPF_f = tf.feature_column.numeric_column("DiabetesPedigreeFunction")

    age_f = tf.feature_column.numeric_column("Age")

    fetcol = [preg_f, gluc_f, bl_f, sk_f, ins_f, bmi_f, DPF_f, age_f]

    return fetcol




























