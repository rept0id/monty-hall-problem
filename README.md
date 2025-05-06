# monty-hall-problem

![](./doc/assets/img/header1-D06M05Y25.png)

A simulation of the Monty Hall Problem.

From Wikipedia :

```
The Monty Hall problem is a brain teaser, in the form of a probability puzzle, based nominally on the American television game show Let's Make a Deal and named after its original host, Monty Hall.

[...]

It became famous as a question from reader Craig F. Whitaker's letter quoted in Marilyn vos Savant's "Ask Marilyn" column in Parade magazine in 1990:

"Suppose you're on a game show, and you're given the choice of three doors: Behind one door is a car; behind the others, goats. You pick a door, say No. 1, and the host, who knows what's behind the doors, opens another door, say No. 3, which has a goat. He then says to you, "Do you want to pick door No. 2?" Is it to your advantage to switch your choice?"

[...]

Savant's response was that the contestant should switch to the other door.

[...]

When the player first makes their choice, there is a ⁠2/3⁠ chance that the car is behind one of the doors not chosen. This probability does not change after the host reveals a goat behind one of the unchosen doors. When the host provides information about the two unchosen doors (revealing that one of them does not have the car behind it), the ⁠2/3⁠ chance of the car being behind one of the unchosen doors rests on the unchosen and unrevealed door, as opposed to the ⁠1/3⁠ chance of the car being behind the door the contestant chose initially.

[...]

**Paul Erdős, one of the most prolific mathematicians in history, remained unconvinced until he was shown a computer simulation demonstrating Savant's predicted result.**
```

## Build

```
make build
```

## Run

```
make run
```

or

```
./monty-hall-problem
```

Example Result :
```
Host Reveal,Player Change,Wins,Curtains,Games
0,0,333455,3,1000000
1,0,333455,3,1000000
0,1,333083,3,1000000
1,1,666544,3,1000000
```

Optional Flags :

`--log-no-header`:
- **Desc.**: Prints result without header.
- **Ex.**: `./monty-hall-problem --log-no-header`

`--curtains <NUMBER>`
- **Short**: `--c <NUMBER>`
- **Desc.**: Changes how many curtains there are.
- **Ex.**: `./monty-hall-problem --curtains 4`, `./monty-hall-problem --c 4`
- **Default** : 3

`--state-games <NUMBER>`
- **Short**: `--sg <NUMBER>`
- **Desc.**: Changes how many games (iterations/loops) per state (cases) there are.
- **Ex.**: `./monty-hall-problem --state-games 2000000`, `./monty-hall-problem --sg 2000000`
- **Default** : 1000000

## Memory-safety

This program, even written in C, tries to provide memory-safety, by using fixed-size stack arrays.

## Special Statistical Cases

Apart from the classic cases, this implementation also includes two additional cases:
- The host doesn't reveal any curtain, and the player keeps the chosen curtain.
  
  (Host Reveal=0,Player Change=0)
  
- The host doesn't reveal any curtain, and the player changes to the other curtain.

  (Host Reveal=0,Player Change=1)

What is common in both cases is that the host doesn't reveal any curtain.

The outcome of this, when you run the simulation, is that **if the host doesn't reveal a curtain, the player's chances of winning remain the same, changing the curtain or not**.

This differs from the classic game, where the host reveals one of the curtains, and if the player changes curtain the chances of winning are higher.

The purpose of all this, is to evaluate that it's the host that by revealing one of the curtains, allows the player to change the propabilities of winning from 1/3 to 2/3 by changing.

## Multiprocessing

This implementation uses **OpenMP** to parallelize the "games" in the simulation.
