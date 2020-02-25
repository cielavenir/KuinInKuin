#!/bin/sh
dir=$(dirname $(realpath $0))
cd $dir
./codes.rb cui_input.cpp
./codes.rb cui_inputChar.cpp
./codes.rb cui_inputFloat.cpp
./codes.rb cui_inputInt.cpp
./codes.rb cui_inputLetter.cpp
./codes.rb cui_inputStr.cpp
./codes.rb math_gcd.cpp a_ b_
./codes.rb math_lcm.cpp a_ b_
./codes.rb math_modMul.cpp a_ b_ modulus_
./codes.rb math_modPow.cpp value_ exponent_ modulus_
./codes.rb math_knapsack.cpp weights_ values_ maxWeight_ reuse_
./codes.rb math_prime.cpp n_
./codes.rb math_primeFactors.cpp n_
./codes.rb math_dijkstra.cpp from_nodes_ to_nodes_ values_ begin_nodes_
