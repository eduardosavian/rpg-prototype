# RPG-Prototype
#### Subject taught by the teacher Rodrigo Lyra

Activities involving structs, pointers and recursion

# Part 1- Structs

An RPG game needs to start somewhere, and an essential part is the battles.
against enemies, and for that we need weapons. Of course at this time we will not
make a complete game, we will only simulate the combat of a phase, for that
we will use what we learned from structs and templates. Below are listed the items
mandatory work:
structs
The work must contain at least the following structs:
▪ Phase – Which must have a name and 5 enemies.
▪ Player – Who must have a level, health and a weapon.
▪ Enemy – Which must have a name, life, and a weapon.
▪ Weapon – Which must have minimum damage and maximum damage
Template
The work must contain at least the following functions with template, the
objective is that the template leaves generic if the variables are of player or enemy (NOT
overloading and no pass-by-reference should be used in these functions).
● died – Which must receive a parameter, which will be the player or an enemy, and
should return a bool symbolizing whether it died or not. The function must check
if the life of whoever was passed by parameter is less than or equal to 0, and return true
if so and false otherwise.
● attack – Which must receive two parameters, the attacker and who will suffer the attack
(which can be a player or enemy), and must return whoever suffered the attack with the
updated life. The life of the one attacked must be diminished by a value
random between the attacker's weapon's minimum damage and maximum damage.
LogicIn addition to the use of templates and structs, at least one more logic function must
be implemented.
● playPhase – This function must receive as a parameter the player and a phase and
should follow the following logic:
o Write the name of the phase;
o For each enemy in the level:
▪ Until the enemy dies:
● Enemy attacks the player
● Player attacks the enemy
● If the player dies:
the game is over
● If the enemy dies:
o Moves to the next enemy
o If the enemies run out, the game is over

# Part 2 - Pointers and Dynamic Allocation

Continuing the ideas from the past work, let's now make a map for our RPG and think about other functions, in it we will need pointers and dynamic allocation, but we will not abandon the concepts discussed above. For today's project
We have some mandatory elements:

We are going to work with a 2D block map, which will represent the scenario of our world.
This map must be dynamic, stored in an AxL array of variables of type block, which will contain some information about the game, based on a struct. It should also have a little gameplay. The minimum functionalities are described in follow. (You can use the previous work as a basis for this one, or the project available on courseware) structs:

The work must contain at least the following structs: 
▪ Weapon;

▪ Player: This struct must contain health, and the x and y positions on the map and a
weapon;

▪ Enemy: This struct must contain a name and health and a weapon;

▪ Block: This struct represents a block on the map, it must contain the information
whether it is blocked or not (path or stone), whether it has an enemy or not, and a pointer to a variable of type Enemy (which can be empty if not have no enemies on the block);


▪ Map: This struct must have a height A, a width L, and an array of variables of type Block AxL (this array must be dynamically allocated, values ​​must not come from template);

▪ Phase: Must contain a name, a Map type variable, a number of enemies N and a vector of size N of variables of type Enemy (also dynamically allocated) Logic.

The work must contain at least the following functions:

▪ Map CreateMap(int height, int width): This function must receive the height and width of the map and should return a new variable of that type dynamically allocated with the size passed by parameter (this allocation must be by the method using continuous memory). each block of that map must have a 20% chance of being blocked (stone) and an 80% chance of not being be blocked (path) and start with no enemies.

▪ Phase CreatePhase(int numEnemies, Enemies* enemies, int heightMap, int mapwidth): This function must receive the number of enemies and an enemy vector and also the height and width of the map. Is it over there must call the CreateMap function to generate the map. Then she must each enemy, add that enemy as a member of a random block of the map, as long as it is not locked and does not yet have any enemy. In the end all enemies must be allocated in positions in the map, so if the drawn position is invalid, a new one must be drawn.

▪ void Move(...): This function should move the player around the map (the user must use the keyboard for this), he must not be able to walk off the map or to a position that is locked. If he walks into a position with an enemy, combat must be initiated. automatic (use previous job function for an enemy). The game ends if the player or the last enemy dies.

# Part 3 - Deep Search

Continuing the map ideas from the past work, let's now make a shape different from generating the world of our game, for this we will use a little allocation dynamics.

We will continue to work with a 2D block map, but now this map will have more obstacles, instead of 20% it must work with 50% stone blocks. At rules for spawning enemies remain the same, but the end conditions of the map must have some changes.

A starting point of the phase must be stipulated, where the character starts, in one of the corners of the map, and an end point of the map, in the corner of the diagonal opposite the beginning. The game ends now when the character reaches the end point of the map and all enemies were killed.

For this to be possible, there must be a walkable path from the beginning of the map to the destination and also for each enemy. For this we will use recursive functions, from beginning to end. end and for each enemy. If there are viable paths for all enemies and the ending, the map is valid and the game can start, otherwise the map must be regenerated. The way to do this verification must be through a recursive algorithm, based on the depth-first search algorithm, where every possible path is checked step by step, until all goals are reached or until all paths possible are checked without success.

