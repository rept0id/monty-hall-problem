# monty-hall-paradox

```gcc main.c -o main```

```
> time ./main
Host Reveal,Player Change,Wins,Curtains,Games
1,1,388689,3,1000000
0,1,332597,3,1000000
1,0,332888,3,1000000
0,1,332507,3,1000000

real	0m0.432s
user	0m0.429s
sys	0m0.002s
```

Optional Flags :
- "--g x", sets num of games per state, e.g. "./main --g 100000". Default is 1000000.
- "--c x", sets num of curtains", e.g "./main --c 4". Default is 3.
- "--log-no-header", prints log without a header.
