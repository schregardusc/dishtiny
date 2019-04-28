# usage:
# dataframe_filename

import sys
import os
import seaborn as sns
import pandas as pd
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
import numpy as np
from keyname import keyname as kn
from fileshash import fileshash as fsh

matplotlib.rcParams['pdf.fonttype'] = 42
sns.set(style='whitegrid')

dataframe_filename = sys.argv[1]

df = pd.read_csv(dataframe_filename)

df['Treatment'] = df['Treatment'].apply(lambda raw : {
    'resource-even__channelsense-no__nlev-two' : 'even+blind',
    'resource-wave__channelsense-no__nlev-two' : 'blind',
    'resource-wave__channelsense-yes__nlev-onesmall' : 'small wave',
    'resource-even__channelsense-yes__nlev-two' : 'even',
    'resource-wave__channelsense-yes__nlev-onebig' : 'large wave',
    'resource-wave__channelsense-yes__nlev-two' : 'standard'
    }[raw]
)

ax = sns.barplot(
    x="Treatment",
    y="Incoming Reproduction Rate",
    hue="Level 1 Channel ID",
    hue_order=[
    data=df
)
plt.xticks(rotation=30)


ax.get_figure().savefig(
    kn.pack({
        'title' : 'reproductive_labor',
        '_data_hathash_hash' : fsh.FilesHash().hash_files([dataframe_filename]),
        '_script_fullcat_hash' : fsh.FilesHash(
                                            file_parcel="full_parcel",
                                            files_join="cat_join"
                                        ).hash_files([sys.argv[0]]),
        '_source_hash' :kn.unpack(filename)['_source_hash'],
        'ext' : '.pdf'
    }),
    transparent=True,
    bbox_inches='tight',
    pad_inches=0
)