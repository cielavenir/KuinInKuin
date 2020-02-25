#!/bin/sh
dir=$(dirname $(realpath $0))
cd $dir
./codes.rb math_gcd.cpp a_ b_
./codes.rb math_lcm.cpp a_ b_
./codes.rb math_modMul.cpp a_ b_ modulus_
./codes.rb math_modPow.cpp value_ exponent_ modulus_
./codes.rb math_knapsack.cpp weights_ values_ maxWeight_ reuse_
./codes.rb math_prime.cpp n_
./codes.rb math_primeFactors.cpp n_
./codes.rb math_fact.cpp n_
./codes.rb math_factInt.cpp n_
./codes.rb math_fibonacci.cpp n_
./codes.rb math_gamma.cpp n_
./codes.rb math_dijkstra.cpp from_nodes_ to_nodes_ values_ begin_nodes_
./codes.rb math_bellmanFord.cpp from_nodes_ to_nodes_ values_ begin_nodes_