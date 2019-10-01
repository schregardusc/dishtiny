# usage: file updates...

import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
from matplotlib.collections import LineCollection
import matplotlib.pyplot as plt
from matplotlib import cm
from matplotlib.colors import ListedColormap, LinearSegmentedColormap

import sys
import os
import pandas as pd
import numpy as np
from keyname import keyname as kn
from fileshash import fileshash as fsh
import h5py
import colorsys
from tqdm import tqdm
from joblib import delayed, Parallel

viridis = cm.get_cmap('viridis', 12)

matplotlib.rcParams['pdf.fonttype'] = 42

filename = sys.argv[1]
updates = (int(v) for v in sys.argv[2:])

def ColorMap(val):
    return viridis(val)

def RenderTriangles(
        top_val,
        bottom_val,
        left_val,
        right_val,
        live_val,
        radius=21
    ):

    b = (1.0, 1.0, 1.0, 1.0)
    w = (0.0, 0.0, 0.0, 1,0)
    top = ColorMap(top_val)
    bottom = ColorMap(bottom_val)
    left = ColorMap(left_val)
    right = ColorMap(right_val)

    res = np.array([
        [b] * (radius * 2)
    ] + [
        [b]
        + [left] * idx
        + [b]
        + [top] * (2 * (radius - idx) - 3)
        + [b]
        + [right] * idx
        for idx in range(radius - 1)
    ] + [
        [b] + [left] * (radius - 1) + [b] + [right] * (radius - 1)
    ] + [
        [b]
        + [left] * (radius - idx - 1)
        + [b]
        + [bottom] * (2 * idx - 1)
        + [b]
        + [right] * (radius - idx - 1)
        for idx in range(1, radius)
    ]) if live_val else np.full((radius, radius), w)

    return res



def RenderAndSave(upd, filename):

    file = h5py.File(filename, 'r')
    nlev = int(file.attrs.get('NLEV'))

    top = np.array(file['ResourceContributed']['dir_0']['upd_'+str(upd)])
    bottom = np.array(file['ResourceContributed']['dir_1']['upd_'+str(upd)])
    left = np.array(file['ResourceContributed']['dir_3']['upd_'+str(upd)])
    right = np.array(file['ResourceContributed']['dir_2']['upd_'+str(upd)])
    live = np.array(file['Live']['upd_'+str(upd)])

    image = np.transpose(np.block([
        [
            np.transpose(RenderTriangles(
                val_top,
                val_bottom,
                val_left,
                val_right,
                val_live
            )) for val_top, val_bottom, val_left, val_right, val_live in zip(
                row_top,
                row_bottom,
                row_left,
                row_right,
                row_live
            )
        ]
        for row_top, row_bottom, row_left, row_right, row_live
        in zip(
            top,
            bottom,
            left,
            right,
            live
        )
    ]))

    plt.figure(figsize=(50,50))

    plt.imshow(
        image,
        extent = (0, image.shape[1], image.shape[0], 0)
    )

    plt.axis('off')
    plt.grid(b=None)

    plt.savefig(
        kn.pack({
            'title' : 'directional_sharing_viz',
            'update' : str(upd),
            'seed' : kn.unpack(filename)['seed'],
            'treat' : kn.unpack(filename)['treat'],
            '_data_hathash_hash' : fsh.FilesHash().hash_files([filename]),
            '_script_fullcat_hash' : fsh.FilesHash(
                                                file_parcel="full_parcel",
                                                files_join="cat_join"
                                            ).hash_files([sys.argv[0]]),
            '_source_hash' :kn.unpack(filename)['_source_hash'],
            'ext' : '.png'
        }),
        transparent=True,
        bbox_inches='tight',
        pad_inches=0
    )

    plt.clf()
    plt.close(plt.gcf())

Parallel(n_jobs=-1)(
    delayed(RenderAndSave)(upd, filename) for upd in tqdm(updates)
)
