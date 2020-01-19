import numpy as np
import pandas as pd
learned_weights=pd.read_csv("weights.csv",header=None)
weight_matrix = np.array(learned_weights.values)
filename="./values.h"
np.savetxt(filename,weight_matrix,fmt="%1.5f",delimiter=',',newline=",\n")
