# Fall-Challenge2024

The project is now official: Selenia City, the first city on the moon, will be inaugurated in 2050!
The definitive layout of the city hasn't been fully determined yet, so the architects are looking for a solution that could best adapt to all possible configurations.
After your victory in the planetary programming games, you were the first to be called to design the artificial intelligence that will develop the first transportation network on the moon.


## Rules

The game is played in 20 lunar months of 20 days each.
At the beginning of each month, you will receive resources as well as the list of new buildings constructed in Selenia City. You will then need to use your resources efficiently to connect the new buildings to the network, or reinforce the existing infrastructure.
Transportation modes
There are two transportation modes on the lunar base: magnetic tubes and teleporters.

## Magnetic tubes

Magnetic tubes are built on the lunar ground and allow transport pods to carry astronauts through them. They are built in a straight line between two buildings and are bidirectional.
Two tubes cannot cross each other, and each building can only be connected to at most 5 magnetic tubes. A magnetic tube cannot pass through a building without stopping there.
Initially, each tube can only accommodate a single pod at a time, which takes an entire day to travel through it (regardless of its length) while carrying a maximum of 10 passengers. It is however possible to upgrade tubes so they can be traversed by several pods simultaneously.
You will have the possibility to configure precisely the path of each transport pod, which can travel freely through the network of tubes.


## Teleporters

Teleporters provide a very efficient way of traveling, although they are more expensive to build than tubes. They allow astronauts to move instantly from a building to another, without limits on how many passengers can use it at once. The path of two teleporters can cross.
Since replication equipment is very bulky, each building can only accommodate a teleporter entrance or exit.

## Buildings

Buildings in Selenia City are organized on a rectangular area of 160 x 90 kilometers. Two types of buildings will exist in the city: lunar modules and landing pads.
Lunar modules

## Lunar modules

the 20 types of lunar modules in Selenia City
There are several different types of lunar modules (laboratory, ground sampling site, observatory, ...) that will permanently house the astronauts.
You need to transport each astronaut to a building of the corresponding type in order to win points. There may exist several modules of the same type in Selenia City.

## Landing pads


astronauts delivered on a landing pad
Landing pads are buildings that work in a special way: at the beginning of each month, a rocket will drop a group of astronauts on each of them. The composition of the group is fixed: if for example a landing pad welcomes 5 lab technicians and 10 observatory employees, an identical group will arrive each month.
If several lunar modules have the same type as the astronaut, he may settle into any of them.
Your solution will score more points if it manages to balance the astronaut population well between modules of the same type

## Scoring

Your objective is to score as many points as possible before the end of the simulation.
Each astronaut reaching its target before the end of the lunar month will give you up to 100 points:
For speed: 50 points, from which are deducted the number of days needed by the astronaut to reach the destination.

For balancing the population: 50 points, from which are deducted the number of astronauts who have already reached the same module during the current lunar month. If it's negative, this score will be brought back to 0.

```
Example 1 : An astronaut of type 4 arrives in Selenia City on the first day of the month, and immediately takes a teleporter that takes her from the landing pad to a building of type 4.
She is the first one to reach her base, you will score 100 points (50 speed points and 50 balancing points).

Example 2 : An astronaut takes a pod through a magnetic tube on the first day to leave his landing pad, then a teleporter followed by another tube on the second day. He reaches his module as 60 astronauts have previously been installed in this building during that month.
He will earn you 48 points (48 speed points and no balancing point). 
```

## End of the lunar month

At the end of each month, all remaining astronauts disappear from the play area and all pods immediately return to their starting point. All unused resources yield a 10% interest (rounded down).
Constraints
During the game, at most 150 buildings will be constructed.
Each landing pad will welcome between 1 and 100 astronauts each month.
At most 1000 astronauts will arrive each month.
It is guaranteed that no building will be constructed on the path of an existing tube.
Your program has to return its list of actions within 500 milliseconds each turn (1000 milliseconds for the first turn).
It is guaranteed that each astronaut arriving on a landing pad will have at least 1 module of the same type already built.
Note: hidden validators, on which the score of your solution is evaluated on the leaderboard, will be changed between the challenge's end and the final score calculation in order to avoid hardcoded solutions. It is however guaranteed that hidden validators will be similar to the visible testcases.
Point-segment intersection
To verify whether building A stands on segment BC, it is possible to create triangle ABC and check if distance BC equals BA+AC.
```python
function distance(p1, p2) {
    return sqrt((p2.x-p1.x)² + (p2.y - p1.y)²)
}
```

```python
function pointOnSegment(A, B, C) {
    epsilon = 0.0000001
    return (-epsilon < distance(B, A) + distance(A, C) - distance(B, C) < epsilon)
}
```

Here, the presence of epsilon is used to compare floating-point numbers despite their limited precision (in most languages, 0.1 + 0.2 != 0.3). There are other methods involving only integers, but it is guaranteed that the algorithm above works for all integer points in the 160x90 grid with the given epsilon value.
Segment-segment intersection
To verify whether two line segments AB and CD cross outside their extremities, you can use the segmentsIntersect function defined below:
```python
function orientation(p1, p2, p3) {
    prod = (p3.y-p1.y) * (p2.x-p1.x) - (p2.y-p1.y) * (p3.x-p1.x)
    return sign(prod)
}
```

```python
function segmentsIntersect(A, B, C, D) {
    return orientation(A, B, C) * orientation(A, B, D) < 0 && orientation(C, D, A) * orientation(C, D, B) < 0
}
```

Here, the function sign(x) returns -1 if x is negative, 0 if it's zero and 1 if it's positive.

