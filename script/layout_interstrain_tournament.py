#!/usr/bin/env python3

import sys

import networkx as nx

seed, degree, *series = map(int, sys.argv[1:])

n = len(series)
G = nx.generators.random_graphs.random_regular_graph(degree, n, seed)

G = nx.relabel_nodes( G, lambda i: series[i] )

for a, b in G.edges:
    print(a, b)
