        xdef    _TestSocketBase
        xref    _TestNames
        xref    _TestFreed
        xref    _TestNullFreed
        xref    _TestBadFree

        section __MERGED,data
; A small test library with exactly six bytes per negative vector.
        dc.w    $4ef9
        dc.l    MockFree
        dc.w    $4ef9
        dc.l    MockList
        dc.w    $4ef9
        dc.l    MockIndex
        dc.w    $4ef9
        dc.l    MockName
        ds.b    876
_TestSocketBase:
        ds.b    16
        dc.w    900
        ds.b    16

        section callbacks,code
MockName:
        cmpi.b  #108,(a0)
        bne.s   Unknown
        cmpi.b  #111,1(a0)
        bne.s   Unknown
        cmpi.b  #48,2(a0)
        bne.s   Unknown
        tst.b   3(a0)
        bne.s   Unknown
        moveq   #7,d0
        rts
Unknown:
        moveq   #0,d0
        rts
MockIndex:
        cmpi.l  #7,d0
        bne.s   Unknown
        move.l  a0,d0
        move.b  #108,(a0)
        move.b  #111,1(a0)
        move.b  #48,2(a0)
        clr.b   3(a0)
        rts
MockList:
        lea     _TestNames,a0
        move.l  a0,d0
        rts
MockFree:
        move.l  a0,d0
        beq.s   FreeNull
        lea     _TestNames,a1
        cmpa.l  a1,a0
        bne.s   FreeBad
        addq.l  #1,_TestFreed
        rts
FreeNull:
        addq.l  #1,_TestNullFreed
        rts
FreeBad:
        addq.l  #1,_TestBadFree
        rts
        end
