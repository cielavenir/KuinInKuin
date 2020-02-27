#!/bin/sh
dir=$(dirname $(realpath $0))
cd $dir
./codes.rb math_gamma.cpp n_
./codes.rb math_dijkstra.cpp from_nodes_ to_nodes_ values_ begin_nodes_
./codes.rb math_bellmanFord.cpp from_nodes_ to_nodes_ values_ begin_nodes_
./codes.rb toStrFmtInt.cpp me_ fmt_
./codes.rb toStrFmtFloat.cpp me_ fmt_