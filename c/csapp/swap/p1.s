000000000040048c <swap>:
  40048c:	55                   	push   %rbp
  40048d:	48 89 e5             	mov    %rsp,%rbp
  400490:	48 c7 05 05 04 20 00 	movq   $0x600880,0x200405(%rip)        # 6008a0 <bufp1>
  400497:	80 08 60 00 
  40049b:	48 8b 05 e6 03 20 00 	mov    0x2003e6(%rip),%rax        # 600888 <bufp0>
  4004a2:	8b 00                	mov    (%rax),%eax
  4004a4:	89 45 fc             	mov    %eax,-0x4(%rbp)
  4004a7:	48 8b 05 da 03 20 00 	mov    0x2003da(%rip),%rax        # 600888 <bufp0>
  4004ae:	48 8b 15 eb 03 20 00 	mov    0x2003eb(%rip),%rdx        # 6008a0 <bufp1>
  4004b5:	8b 12                	mov    (%rdx),%edx
  4004b7:	89 10                	mov    %edx,(%rax)
  4004b9:	48 8b 05 e0 03 20 00 	mov    0x2003e0(%rip),%rax        # 6008a0 <bufp1>
  4004c0:	8b 55 fc             	mov    -0x4(%rbp),%edx
  4004c3:	89 10                	mov    %edx,(%rax)
  4004c5:	c9                   	leaveq 
  4004c6:	c3                   	retq   
  4004c7:	90                   	nop
  4004c8:	90                   	nop
  4004c9:	90                   	nop
  4004ca:	90                   	nop
  4004cb:	90                   	nop
  4004cc:	90                   	nop
  4004cd:	90                   	nop
  4004ce:	90                   	nop
  4004cf:	90                   	nop

