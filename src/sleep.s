.text
.global sleep
sleep:
    beqz a0, 2f
1:
    addi a0, a0, -1
    bnez a0, 1b
2:
    ret
