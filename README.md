# RobotGenetics_CPP
FLC CISP400 Final for C++

## Problem

You are going to reproduce an experiment first conducted at Harvard University in 1968. This program studies the effects of evolution on a population of robots. The robots need to maneuver around a grid collecting energy. The robots must collect more energy than they expend to survive. Your robots will maneuver around a 10 x 10 grid looking for batteries. A battery gives the robot give more power. Moving a square costs 1 power. The sensors are always on and cost no power. Robots have five power when they first emerge on the map.

Each Robot has a collection of direction sensors and a motor. Robot success depends upon the map between these two elements. The robots start with random mapping, but over time, the mappings evolve into successful strategies. The top 50% of the best performing robots each generation will "mate" and give two offspring with an equally random collection of both parents genes. This should increase the overall fitness score of the robots as the generations increase.

## Output

The output of this program consists of 100 numbers, one for each generation of Robot. It contains the average fitness score of each robot in the generation. The fitness score is derived from the amount of moves each robot makes before running out of battery.
