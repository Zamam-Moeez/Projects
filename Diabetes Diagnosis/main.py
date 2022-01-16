import numpy as np
import pandas as pd
import tensorflow as tf
from tensorflow import feature_column
from tensorflow.keras import layers
from sklearn.model_selection import train_test_split

def convert_dtype(x):

    if not x:

        return ''

    try:

        return str(x)

    except:

        return ''

def pre_process():

    dataf = pd.read_csv('diabetes.csv',converters={'first_column': convert_dtype,'second_column': convert_dtype})

    columns = ["Pregnancies", "Glucose", "BloodPressure", "SkinThickness", "Insulin",

       "BMI", "DiabetesPedigreeFunction", "Age", "Outcome"]

    datanorm=(dataf-dataf.min())/(dataf.max()-dataf.min())

    train, test = train_test_split(datanorm, test_size=0.2, random_state= 42)

    train, val = train_test_split(train, test_size=0.2, random_state= 42)

    return  train, test, val

def df_to_dataset(dataframe, shuffle=True, batch_size=32):

  dataframe = dataframe.copy()

  labels = dataframe.pop('Outcome')

  ds = tf.data.Dataset.from_tensor_slices((dict(dataframe), labels))

  if shuffle:

    ds = ds.shuffle(buffer_size=len(dataframe))

  ds = ds.batch(batch_size)

  return ds

def features():

    fetcol = []

    for header in ['Pregnancies', 'Glucose', 'BloodPressure', 'SkinThickness', 'Insulin',

       'BMI', 'DiabetesPedigreeFunction', 'Age'] :

        fetcol.append(feature_column.numeric_column(header))

    return fetcol

def model():

    fetcol = features()

    fetlay = tf.keras.layers.DenseFeatures(fetcol)

    batch_size = 32
    trains = df_to_dataset(pre_process()[0], batch_size=batch_size)
    tests = df_to_dataset(pre_process()[1], shuffle=False, batch_size=batch_size)
    valds = df_to_dataset(pre_process()[2], shuffle=False, batch_size=batch_size)

    model = tf.keras.Sequential([

        fetlay,

        layers.Dense(128, activation='relu'),

        layers.Dense(128, activation='relu'),

        layers.Dropout(.1),

        layers.Dense(1)
    ])

    model.compile(optimizer='adam',
                  loss=tf.keras.losses.BinaryCrossentropy(from_logits=True),
                  metrics=['accuracy'])
    model.fit(trains,
              validation_data=valds,
              epochs=128)

    loss, accuracy = model.evaluate(tests)

    print("Accuracy", accuracy)

if __name__ == "__main__":

    model()





























