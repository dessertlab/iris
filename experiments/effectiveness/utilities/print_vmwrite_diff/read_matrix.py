from pydoc import doc
import pandas as pd
import numpy as np



namefile_mon="./seeds_matrix_mon" 
namefile_inj="./seeds_matrix_inj"

mon_file = np.loadtxt(namefile_mon, dtype='double', delimiter=',')
inj_file = np.loadtxt(namefile_inj, dtype='double', delimiter=',')

mon_df = pd.DataFrame(mon_file)
inj_df = pd.DataFrame(inj_file)

diff_df = mon_df-inj_df
diff_df = diff_df.loc[~(diff_df==0).all(axis=1)]

print(diff_df)
#np.savetxt(namefile_out,newdf.index.values, delimiter=',', fmt='% d')
