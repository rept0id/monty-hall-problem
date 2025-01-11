# monty-hall-paradox

## Build

```gcc main.c -o main```

## Run

```
> ./main
Host Reveal,Player Change,Wins,Curtains,Games
1,1,388689,3,1000000
0,1,332597,3,1000000
1,0,332888,3,1000000
0,1,332507,3,1000000
```

Optional Flags :
- "--log-no-header", prints log without a header.
- "--c x", sets num of curtains, e.g "./main --c 4". Default is 3.
- "--sg x", sets num of games per state, e.g. "./main --g 100000". Default is 1000000.
