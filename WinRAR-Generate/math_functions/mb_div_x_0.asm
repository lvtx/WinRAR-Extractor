;/*********************************************************************
;* Filename:   _mb_div_x.asm
;* Author:     Aiyu Xiao (xiao_ai_yu@live.cn, OR doublesine@tju.edu.cn)
;*********************************************************************/
TITLE mb_div_x.asm

;-------------------------------
;           FOR X64
;-------------------------------
IFDEF _M_X64
.CODE
;   Undecorate symbol name is
;   unsigned __int64 __fastcall accel::math::mb_div_u64(unsigned __int64 dividend[], unsigned __int64 length, unsigned __int64 divisor)
;   rcx = src
;   rdx = length
;   r8  = divisor
?mb_div_u64@math@accel@@YA_KQEA_K_K1@Z PROC
    test rdx, rdx       ;   if (rdx(length) == 0) {
    jnz start_div       ;
    div r8              ;       rax = rdx:rax / r8; rdx = rdx:rax % r8;  ### if r8 is zero, here can triggle a divided-by-zero exception.
                        ;                                                ### so if divisor is zero, the exception will reveal that there is a bug.
    mov rax, 0FFFFFFFFFFFFFFFFH ;
    ret                 ;       return 0xffffffffffffffff;               ### to tell caller that arguments are invalid.
start_div:              ;   } else {
    push rsi            ;       save rsi;

    mov rsi, rcx        ;       rsi = rcx(src);
    mov rcx, rdx        ;       rcx = rdx(length);
    mov rax, rdx        ;       rax = rdx(length);
    shl rax, 3          ;       rax *= 8;               ### rax = 8 * length
    add rsi, rax        ;       rsi += rax
    jnc no_overflow     ;       if (carry_flag == 1) {
    pop rsi             ;           recover rsi;
    xor rax, rax        ;           return 0;
    ret                 ;       }
no_overflow:            ;
    std                 ;       flags.DF = 1;
    xor rdx, rdx        ;       rdx = 0;
    sub rsi, 8          ;       rsi -= 8;               ### let rsi point to the last integer in src.
cycle0:                 ;       do {
    lodsq               ;           rax = qword ptr[rsi];   rsi -= 8;
    div r8              ;           rax = rdx:rax / r8(divisor), rdx = rdx:rax % r8(divisor);
    mov qword ptr[rsi + 8], rax ;   qword ptr[rsi + 8] = rax;
    loop cycle0         ;           rcx--;
                        ;       } while(rcx != 0);
    mov rax, rdx        ;       rax = rdx;
    cld                 ;       flags.DF = 0;           ### According to MSDN: https://msdn.microsoft.com/en-us/library/7td56tzs(v=vs.140).aspx, 
                        ;                               ### it should be cleared.
    pop rsi             ;   }
    ret
?mb_div_u64@math@accel@@YA_KQEA_K_K1@Z ENDP

;   Undecorate symbol name is
;   unsigned __int64 __fastcall accel::math::_udiv128(uint64_t dividend_l, uint64_t dividend_h, 
;                                                     uint64_t divisor, 
;                                                     uint64_t* remainder)
;   rcx = dividend_l
;   rdx = dividend_h
;   r8  = divisor
;   r9  = remainder
?_udiv128@math@accel@@YA_K_K00PEA_K@Z PROC
    mov rax, rcx                ;   rax = rcx(dividend_l);
    div r8                      ;   rdx:rax(dividend_h:dividend_l) / r8(divisor);
    test r9, r9                 ;   if (remainder != nullptr) {
    jz not_need_remainder       ;       
    mov qword ptr[r9], rdx      ;       *remainder = rdx;
not_need_remainder:             ;   }
    ret                         ;   return rax;
?_udiv128@math@accel@@YA_K_K00PEA_K@Z ENDP

;   Undecorate symbol name is
;   unsigned __int64 __fastcall accel::math::u128_div_u64(const uint64_t (&dividend)[2], 
;                                                         uint64_t divisor)
;   rcx = dividend
;   rdx = divisor
;   
;   label prefix: F2
?u128_div_u64@math@accel@@YA_KAEAY01$$CB_K_K@Z PROC
    mov r8, rdx                     ;   r8 = rdx(divisor);
    cmp qword ptr[rcx + 8], rdx     ;   if (dividend[1] < rdx(divisor)) {
    jnb F2_condition1               ;
F2_condition0:                      ;
    mov rax, qword ptr[rcx]         ;       rax = dividend[0];
    mov rdx, qword ptr[rcx + 8]     ;       rdx = dividend[1];
    div r8                          ;       rdx:rax(dividend[1]:dividend[0]) / r8(divisor);
    mov rax, rdx                    ;       rax = rdx;
    ret                             ;       return rax;
F2_condition1:                      ;   } else {
    mov rax, qword ptr[rcx + 8]     ;       rax = dividend[1];
    xor rdx, rdx                    ;       rdx = 0;
    div r8                          ;       rdx:rax(0:dividend[1]) / r8(divisor);
    mov rax, qword ptr[rcx]         ;       rax = dividend[0];
    div r8                          ;       rdx:rax / r8(divisor);
    mov rax, rdx                    ;       rax = rdx;
    ret                             ;       return rax
                                    ;   }
?u128_div_u64@math@accel@@YA_KAEAY01$$CB_K_K@Z ENDP

;   Undecorate symbol name is
;   unsigned __int64 __fastcall accel::math::u128_div_u64(const uint64_t (&dividend)[2], 
;                                                         uint64_t divisor,  
;                                                         uint64_t (&quotient)[2])
;   rcx = dividend
;   rdx = divisor
;   r8  = quotient
;
;   label prefix: F3
?u128_div_u64@math@accel@@YA_KAEAY01$$CB_K_KAEAY01_K@Z PROC
    mov r9, rdx                     ;   r9 = rdx(divisor);
    cmp qword ptr[rcx + 8], rdx     ;   if (dividend[1] < rdx(divisor)) {
    jnb F3_condition1               ;
F3_condition0:                      ;
    mov rax, qword ptr[rcx]         ;       rax = dividend[0];
    mov rdx, qword ptr[rcx + 8]     ;       rdx = dividend[1];
    div r9                          ;       rdx:rax(dividend[1]:dividend[0]) / r9(divisor);
    mov qword ptr[r8], rax          ;       quotient[0] = rax;
    mov qword ptr[r8 + 8], 0        ;       quotient[1] = 0;
    mov rax, rdx                    ;       rax = rdx;
    ret                             ;       return rax;
F3_condition1:                      ;   } else {
    mov rax, qword ptr[rcx + 8]     ;       rax = dividend[1];
    xor rdx, rdx                    ;       rdx = 0;
    div r9                          ;       rdx:rax(0:dividend[1]) / r9(divisor);
    mov r10, rax                    ;       r10 = rax;
    mov rax, qword ptr[rcx]         ;       rax = dividend[0];
    div r9                          ;       rdx:rax / r9(divisor);
    mov qword ptr[r8], rax          ;       quotient[0] = rax;
    mov qword ptr[r8 + 8], r10      ;       quotient[1] = r10;
    mov rax, rdx                    ;       rax = rdx;
    ret                             ;       return rax;
                                    ;   }
?u128_div_u64@math@accel@@YA_KAEAY01$$CB_K_KAEAY01_K@Z ENDP

;   Undecorate symbol name is
;   void __fastcall accel::math::u128_div_u128(const uint64_t (&dividend)[2], 
;                                              const uint64_t (&divisor)[2],
;                                              uint64_t (&quotient)[2], 
;                                              uint64_t (&remainder)[2]);
;   rcx = dividend
;   rdx = divisor
;   r8  = quotient
;   r9 = remainder
;
;   label prefix: F4
?u128_div_u128@math@accel@@YAXAEAY01$$CB_K0AEAY01_K1@Z PROC
    mov rax, qword ptr[rdx + 8]
    test rax, rax
    jnz F4_long_division
    push r9
    mov rdx, qword ptr[rdx]
    call ?u128_div_u64@math@accel@@YA_KAEAY01$$CB_K_KAEAY01_K@Z
    pop r9
    mov qword ptr[r9], rax
    mov qword ptr[r9 + 8], 0
    ret
F4_long_division:
    push rsi
    push rdi
    mov rsi, r8
    mov rdi, r9
    mov r8, rcx
    mov r9, rdx

    mov rax, qword ptr[r8]
    mov rdx, qword ptr[r8 + 8]
    mov r10, qword ptr[r9]
    mov r11, qword ptr[r9 + 8]

    bsr rcx, r11
    inc rcx

    shrd r10, r11, cl
    shrd rax, rdx, cl
    shr rdx, cl

    div r10
    mov rcx, rax
    mul r11
    mov r10, rax
    mov rax, qword ptr[r9]
    mul rcx
    add rdx, r10
    jnc F4_no_carry
    dec rcx
    sub rax, qword ptr[r9]
    sbb rdx, r11
F4_no_carry:
    mov r10, rax
    mov r11, rdx
    mov rax, qword ptr[r8]
    mov rdx, qword ptr[r8 + 8]
    sub rax, r10
    sbb rdx, r11
    jnc F4_all_done
    add rax, qword ptr[r9]
    adc rdx, qword ptr[r9 + 8]
    dec rcx
F4_all_done:
    mov qword ptr[rsi], rcx
    mov qword ptr[rsi + 8], 0
    mov qword ptr[rdi], rax
    mov qword ptr[rdi + 8], rdx
    
    pop rdi
    pop rsi
    ret
?u128_div_u128@math@accel@@YAXAEAY01$$CB_K0AEAY01_K1@Z ENDP

;   Undecorate symbol name is
;   void __fastcall accel::math::u128_div_u128(const uint64_t (&dividend)[2], 
;                                              const uint64_t (&divisor)[2],
;                                              uint64_t (&quotient)[2]);
;   rcx = dividend
;   rdx = divisor
;   r8  = quotient
;
;   label prefix: F5
?u128_div_u128@math@accel@@YAXAEAY01$$CB_K0AEAY01_K@Z PROC
    mov rax, qword ptr[rdx + 8]
    test rax, rax
    jnz F5_long_division
    mov rdx, qword ptr[rdx]
    call ?u128_div_u64@math@accel@@YA_KAEAY01$$CB_K_KAEAY01_K@Z
    ret
F5_long_division:
    push rsi
    push rdi

    mov rsi, rcx
    mov rdi, rdx

    mov rax, qword ptr[rsi]
    mov rdx, qword ptr[rsi + 8]
    mov r9, qword ptr[rdi]
    mov r10, qword ptr[rdi]
    mov r11, qword ptr[rdi + 8]

    bsr rcx, r11
    inc rcx

    shrd r10, r11, cl
    shrd rax, rdx, cl
    shr rdx, cl

    div r10
    mov rcx, rax

    mul r11
    mov r10, rax
    mov rax, r9
    mul rcx
    add rdx, r10
    jc F5_with_carry
    cmp rdx, qword ptr[rsi + 8]
    ja F5_with_carry
    jb F5_all_done
    cmp rax, qword ptr[rsi]
    jbe F5_all_done
F5_with_carry:
    dec rcx
F5_all_done:
    mov qword ptr[r8], rcx
    mov qword ptr[r8 + 8], 0

    pop rdi
    pop rsi
    ret
?u128_div_u128@math@accel@@YAXAEAY01$$CB_K0AEAY01_K@Z ENDP

ENDIF

;-------------------------------
;           FOR X86
;-------------------------------
IFDEF _M_IX86
.MODEL FLAT
.CODE
;   Undecorate symbol name is
;   unsigned __int64 __fastcall accel::math::mb_div_u32(unsigned __int32 src[], unsigned __int32 length, unsigned __int32 divisor)
;   ecx = src
;   edx = length
;   [esp + 4] = divisor
?mb_div_u32@math@accel@@YIIQAIII@Z PROC
            test edx, edx
            jnz _start_div
            div dword ptr[esp + 4]
            mov eax, 0ffffffffh
            ret 4
_start_div:
            push esi
            
            mov esi, ecx
            mov ecx, edx
            mov eax, edx
            shl eax, 2
            add esi, eax
            jnc _no_overflow
            pop esi
            xor eax, eax
            ret 4
_no_overflow:
            std
            xor edx, edx
            sub esi, 4
_cycle0:
            lodsd
            div dword ptr[esp + 4 + 4]
            mov dword ptr[esi + 4], eax
            loop _cycle0

            mov eax, edx
            cld

            pop esi
            ret 4
?mb_div_u32@math@accel@@YIIQAIII@Z ENDP

ENDIF

END
