### Output of Test Case 1

sent p1 msg1 p2 (1)
received p2 msg1 p1 (2)
printed p1 P1 (2)
printed p2 P2 (3)
sent p2 msg2 p1 (4)
received p1 msg2 p2 (5)
printed p2 P2_done (5)
printed p1 P1_done (6)

### Output of Test Case 2

system deadlocked

### Output of Test Case 3

sent p1 m1 p2 (1)
printed p2 x1 (1)
printed p1 abc (2)
received p2 m1 p1 (3)
printed p1 def (3)
printed p2 x2 (4)
sent p2 m2 p1 (5)
printed p2 x3 (6)
message m2 (p2 -> p1) not delivered