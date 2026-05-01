### Test Case 1 — Normal Execution

begin process p1
send p2 m1
print abc
print def
end process

begin process p2
print x1
recv p1 m1
print x2
print x3
end process

### Test Case 2 — Deadlock

begin process p1
recv p2 msgX
print P1
end process

begin process p2
recv p1 msgY
print P2
end process

### Test Case 3 — Undelivered Messages

begin process p1 
send p2 m1 
print abc 
print def 
end process 

begin process p2 
print x1 
recv p1 m1 
print x2 
send p1 m2 
print x3 
end process 
