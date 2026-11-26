/*
 * This file has been modified by L,Studio.
 * Copyright (c) 2026 L,Studio. All rights reserved.
 *
 * Complete 256-entry IDT stub table. Unhandled vectors fail-stop until
 * architecture-specific exception and interrupt dispatch is installed.
 */
.section .text.exceptions,"ax",@progbits
.code64
.extern xul_interrupt_dispatch
.global xul_x86_64_isr_0
.type xul_x86_64_isr_0,@function
xul_x86_64_isr_0:
    pushq $0
    pushq $0
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_1
.type xul_x86_64_isr_1,@function
xul_x86_64_isr_1:
    pushq $0
    pushq $1
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_2
.type xul_x86_64_isr_2,@function
xul_x86_64_isr_2:
    pushq $0
    pushq $2
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_3
.type xul_x86_64_isr_3,@function
xul_x86_64_isr_3:
    pushq $0
    pushq $3
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_4
.type xul_x86_64_isr_4,@function
xul_x86_64_isr_4:
    pushq $0
    pushq $4
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_5
.type xul_x86_64_isr_5,@function
xul_x86_64_isr_5:
    pushq $0
    pushq $5
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_6
.type xul_x86_64_isr_6,@function
xul_x86_64_isr_6:
    pushq $0
    pushq $6
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_7
.type xul_x86_64_isr_7,@function
xul_x86_64_isr_7:
    pushq $0
    pushq $7
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_8
.type xul_x86_64_isr_8,@function
xul_x86_64_isr_8:
    pushq $8
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_9
.type xul_x86_64_isr_9,@function
xul_x86_64_isr_9:
    pushq $0
    pushq $9
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_10
.type xul_x86_64_isr_10,@function
xul_x86_64_isr_10:
    pushq $10
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_11
.type xul_x86_64_isr_11,@function
xul_x86_64_isr_11:
    pushq $11
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_12
.type xul_x86_64_isr_12,@function
xul_x86_64_isr_12:
    pushq $12
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_13
.type xul_x86_64_isr_13,@function
xul_x86_64_isr_13:
    pushq $13
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_14
.type xul_x86_64_isr_14,@function
xul_x86_64_isr_14:
    pushq $14
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_15
.type xul_x86_64_isr_15,@function
xul_x86_64_isr_15:
    pushq $0
    pushq $15
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_16
.type xul_x86_64_isr_16,@function
xul_x86_64_isr_16:
    pushq $0
    pushq $16
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_17
.type xul_x86_64_isr_17,@function
xul_x86_64_isr_17:
    pushq $17
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_18
.type xul_x86_64_isr_18,@function
xul_x86_64_isr_18:
    pushq $0
    pushq $18
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_19
.type xul_x86_64_isr_19,@function
xul_x86_64_isr_19:
    pushq $0
    pushq $19
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_20
.type xul_x86_64_isr_20,@function
xul_x86_64_isr_20:
    pushq $0
    pushq $20
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_21
.type xul_x86_64_isr_21,@function
xul_x86_64_isr_21:
    pushq $21
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_22
.type xul_x86_64_isr_22,@function
xul_x86_64_isr_22:
    pushq $0
    pushq $22
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_23
.type xul_x86_64_isr_23,@function
xul_x86_64_isr_23:
    pushq $0
    pushq $23
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_24
.type xul_x86_64_isr_24,@function
xul_x86_64_isr_24:
    pushq $0
    pushq $24
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_25
.type xul_x86_64_isr_25,@function
xul_x86_64_isr_25:
    pushq $0
    pushq $25
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_26
.type xul_x86_64_isr_26,@function
xul_x86_64_isr_26:
    pushq $0
    pushq $26
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_27
.type xul_x86_64_isr_27,@function
xul_x86_64_isr_27:
    pushq $0
    pushq $27
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_28
.type xul_x86_64_isr_28,@function
xul_x86_64_isr_28:
    pushq $0
    pushq $28
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_29
.type xul_x86_64_isr_29,@function
xul_x86_64_isr_29:
    pushq $29
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_30
.type xul_x86_64_isr_30,@function
xul_x86_64_isr_30:
    pushq $30
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_31
.type xul_x86_64_isr_31,@function
xul_x86_64_isr_31:
    pushq $0
    pushq $31
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_32
.type xul_x86_64_isr_32,@function
xul_x86_64_isr_32:
    pushq $0
    pushq $32
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_33
.type xul_x86_64_isr_33,@function
xul_x86_64_isr_33:
    pushq $0
    pushq $33
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_34
.type xul_x86_64_isr_34,@function
xul_x86_64_isr_34:
    pushq $0
    pushq $34
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_35
.type xul_x86_64_isr_35,@function
xul_x86_64_isr_35:
    pushq $0
    pushq $35
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_36
.type xul_x86_64_isr_36,@function
xul_x86_64_isr_36:
    pushq $0
    pushq $36
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_37
.type xul_x86_64_isr_37,@function
xul_x86_64_isr_37:
    pushq $0
    pushq $37
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_38
.type xul_x86_64_isr_38,@function
xul_x86_64_isr_38:
    pushq $0
    pushq $38
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_39
.type xul_x86_64_isr_39,@function
xul_x86_64_isr_39:
    pushq $0
    pushq $39
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_40
.type xul_x86_64_isr_40,@function
xul_x86_64_isr_40:
    pushq $0
    pushq $40
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_41
.type xul_x86_64_isr_41,@function
xul_x86_64_isr_41:
    pushq $0
    pushq $41
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_42
.type xul_x86_64_isr_42,@function
xul_x86_64_isr_42:
    pushq $0
    pushq $42
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_43
.type xul_x86_64_isr_43,@function
xul_x86_64_isr_43:
    pushq $0
    pushq $43
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_44
.type xul_x86_64_isr_44,@function
xul_x86_64_isr_44:
    pushq $0
    pushq $44
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_45
.type xul_x86_64_isr_45,@function
xul_x86_64_isr_45:
    pushq $0
    pushq $45
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_46
.type xul_x86_64_isr_46,@function
xul_x86_64_isr_46:
    pushq $0
    pushq $46
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_47
.type xul_x86_64_isr_47,@function
xul_x86_64_isr_47:
    pushq $0
    pushq $47
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_48
.type xul_x86_64_isr_48,@function
xul_x86_64_isr_48:
    pushq $0
    pushq $48
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_49
.type xul_x86_64_isr_49,@function
xul_x86_64_isr_49:
    pushq $0
    pushq $49
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_50
.type xul_x86_64_isr_50,@function
xul_x86_64_isr_50:
    pushq $0
    pushq $50
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_51
.type xul_x86_64_isr_51,@function
xul_x86_64_isr_51:
    pushq $0
    pushq $51
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_52
.type xul_x86_64_isr_52,@function
xul_x86_64_isr_52:
    pushq $0
    pushq $52
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_53
.type xul_x86_64_isr_53,@function
xul_x86_64_isr_53:
    pushq $0
    pushq $53
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_54
.type xul_x86_64_isr_54,@function
xul_x86_64_isr_54:
    pushq $0
    pushq $54
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_55
.type xul_x86_64_isr_55,@function
xul_x86_64_isr_55:
    pushq $0
    pushq $55
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_56
.type xul_x86_64_isr_56,@function
xul_x86_64_isr_56:
    pushq $0
    pushq $56
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_57
.type xul_x86_64_isr_57,@function
xul_x86_64_isr_57:
    pushq $0
    pushq $57
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_58
.type xul_x86_64_isr_58,@function
xul_x86_64_isr_58:
    pushq $0
    pushq $58
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_59
.type xul_x86_64_isr_59,@function
xul_x86_64_isr_59:
    pushq $0
    pushq $59
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_60
.type xul_x86_64_isr_60,@function
xul_x86_64_isr_60:
    pushq $0
    pushq $60
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_61
.type xul_x86_64_isr_61,@function
xul_x86_64_isr_61:
    pushq $0
    pushq $61
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_62
.type xul_x86_64_isr_62,@function
xul_x86_64_isr_62:
    pushq $0
    pushq $62
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_63
.type xul_x86_64_isr_63,@function
xul_x86_64_isr_63:
    pushq $0
    pushq $63
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_64
.type xul_x86_64_isr_64,@function
xul_x86_64_isr_64:
    pushq $0
    pushq $64
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_65
.type xul_x86_64_isr_65,@function
xul_x86_64_isr_65:
    pushq $0
    pushq $65
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_66
.type xul_x86_64_isr_66,@function
xul_x86_64_isr_66:
    pushq $0
    pushq $66
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_67
.type xul_x86_64_isr_67,@function
xul_x86_64_isr_67:
    pushq $0
    pushq $67
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_68
.type xul_x86_64_isr_68,@function
xul_x86_64_isr_68:
    pushq $0
    pushq $68
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_69
.type xul_x86_64_isr_69,@function
xul_x86_64_isr_69:
    pushq $0
    pushq $69
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_70
.type xul_x86_64_isr_70,@function
xul_x86_64_isr_70:
    pushq $0
    pushq $70
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_71
.type xul_x86_64_isr_71,@function
xul_x86_64_isr_71:
    pushq $0
    pushq $71
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_72
.type xul_x86_64_isr_72,@function
xul_x86_64_isr_72:
    pushq $0
    pushq $72
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_73
.type xul_x86_64_isr_73,@function
xul_x86_64_isr_73:
    pushq $0
    pushq $73
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_74
.type xul_x86_64_isr_74,@function
xul_x86_64_isr_74:
    pushq $0
    pushq $74
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_75
.type xul_x86_64_isr_75,@function
xul_x86_64_isr_75:
    pushq $0
    pushq $75
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_76
.type xul_x86_64_isr_76,@function
xul_x86_64_isr_76:
    pushq $0
    pushq $76
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_77
.type xul_x86_64_isr_77,@function
xul_x86_64_isr_77:
    pushq $0
    pushq $77
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_78
.type xul_x86_64_isr_78,@function
xul_x86_64_isr_78:
    pushq $0
    pushq $78
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_79
.type xul_x86_64_isr_79,@function
xul_x86_64_isr_79:
    pushq $0
    pushq $79
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_80
.type xul_x86_64_isr_80,@function
xul_x86_64_isr_80:
    pushq $0
    pushq $80
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_81
.type xul_x86_64_isr_81,@function
xul_x86_64_isr_81:
    pushq $0
    pushq $81
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_82
.type xul_x86_64_isr_82,@function
xul_x86_64_isr_82:
    pushq $0
    pushq $82
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_83
.type xul_x86_64_isr_83,@function
xul_x86_64_isr_83:
    pushq $0
    pushq $83
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_84
.type xul_x86_64_isr_84,@function
xul_x86_64_isr_84:
    pushq $0
    pushq $84
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_85
.type xul_x86_64_isr_85,@function
xul_x86_64_isr_85:
    pushq $0
    pushq $85
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_86
.type xul_x86_64_isr_86,@function
xul_x86_64_isr_86:
    pushq $0
    pushq $86
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_87
.type xul_x86_64_isr_87,@function
xul_x86_64_isr_87:
    pushq $0
    pushq $87
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_88
.type xul_x86_64_isr_88,@function
xul_x86_64_isr_88:
    pushq $0
    pushq $88
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_89
.type xul_x86_64_isr_89,@function
xul_x86_64_isr_89:
    pushq $0
    pushq $89
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_90
.type xul_x86_64_isr_90,@function
xul_x86_64_isr_90:
    pushq $0
    pushq $90
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_91
.type xul_x86_64_isr_91,@function
xul_x86_64_isr_91:
    pushq $0
    pushq $91
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_92
.type xul_x86_64_isr_92,@function
xul_x86_64_isr_92:
    pushq $0
    pushq $92
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_93
.type xul_x86_64_isr_93,@function
xul_x86_64_isr_93:
    pushq $0
    pushq $93
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_94
.type xul_x86_64_isr_94,@function
xul_x86_64_isr_94:
    pushq $0
    pushq $94
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_95
.type xul_x86_64_isr_95,@function
xul_x86_64_isr_95:
    pushq $0
    pushq $95
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_96
.type xul_x86_64_isr_96,@function
xul_x86_64_isr_96:
    pushq $0
    pushq $96
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_97
.type xul_x86_64_isr_97,@function
xul_x86_64_isr_97:
    pushq $0
    pushq $97
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_98
.type xul_x86_64_isr_98,@function
xul_x86_64_isr_98:
    pushq $0
    pushq $98
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_99
.type xul_x86_64_isr_99,@function
xul_x86_64_isr_99:
    pushq $0
    pushq $99
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_100
.type xul_x86_64_isr_100,@function
xul_x86_64_isr_100:
    pushq $0
    pushq $100
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_101
.type xul_x86_64_isr_101,@function
xul_x86_64_isr_101:
    pushq $0
    pushq $101
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_102
.type xul_x86_64_isr_102,@function
xul_x86_64_isr_102:
    pushq $0
    pushq $102
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_103
.type xul_x86_64_isr_103,@function
xul_x86_64_isr_103:
    pushq $0
    pushq $103
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_104
.type xul_x86_64_isr_104,@function
xul_x86_64_isr_104:
    pushq $0
    pushq $104
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_105
.type xul_x86_64_isr_105,@function
xul_x86_64_isr_105:
    pushq $0
    pushq $105
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_106
.type xul_x86_64_isr_106,@function
xul_x86_64_isr_106:
    pushq $0
    pushq $106
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_107
.type xul_x86_64_isr_107,@function
xul_x86_64_isr_107:
    pushq $0
    pushq $107
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_108
.type xul_x86_64_isr_108,@function
xul_x86_64_isr_108:
    pushq $0
    pushq $108
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_109
.type xul_x86_64_isr_109,@function
xul_x86_64_isr_109:
    pushq $0
    pushq $109
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_110
.type xul_x86_64_isr_110,@function
xul_x86_64_isr_110:
    pushq $0
    pushq $110
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_111
.type xul_x86_64_isr_111,@function
xul_x86_64_isr_111:
    pushq $0
    pushq $111
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_112
.type xul_x86_64_isr_112,@function
xul_x86_64_isr_112:
    pushq $0
    pushq $112
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_113
.type xul_x86_64_isr_113,@function
xul_x86_64_isr_113:
    pushq $0
    pushq $113
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_114
.type xul_x86_64_isr_114,@function
xul_x86_64_isr_114:
    pushq $0
    pushq $114
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_115
.type xul_x86_64_isr_115,@function
xul_x86_64_isr_115:
    pushq $0
    pushq $115
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_116
.type xul_x86_64_isr_116,@function
xul_x86_64_isr_116:
    pushq $0
    pushq $116
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_117
.type xul_x86_64_isr_117,@function
xul_x86_64_isr_117:
    pushq $0
    pushq $117
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_118
.type xul_x86_64_isr_118,@function
xul_x86_64_isr_118:
    pushq $0
    pushq $118
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_119
.type xul_x86_64_isr_119,@function
xul_x86_64_isr_119:
    pushq $0
    pushq $119
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_120
.type xul_x86_64_isr_120,@function
xul_x86_64_isr_120:
    pushq $0
    pushq $120
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_121
.type xul_x86_64_isr_121,@function
xul_x86_64_isr_121:
    pushq $0
    pushq $121
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_122
.type xul_x86_64_isr_122,@function
xul_x86_64_isr_122:
    pushq $0
    pushq $122
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_123
.type xul_x86_64_isr_123,@function
xul_x86_64_isr_123:
    pushq $0
    pushq $123
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_124
.type xul_x86_64_isr_124,@function
xul_x86_64_isr_124:
    pushq $0
    pushq $124
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_125
.type xul_x86_64_isr_125,@function
xul_x86_64_isr_125:
    pushq $0
    pushq $125
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_126
.type xul_x86_64_isr_126,@function
xul_x86_64_isr_126:
    pushq $0
    pushq $126
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_127
.type xul_x86_64_isr_127,@function
xul_x86_64_isr_127:
    pushq $0
    pushq $127
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_128
.type xul_x86_64_isr_128,@function
xul_x86_64_isr_128:
    pushq $0
    pushq $128
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_129
.type xul_x86_64_isr_129,@function
xul_x86_64_isr_129:
    pushq $0
    pushq $129
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_130
.type xul_x86_64_isr_130,@function
xul_x86_64_isr_130:
    pushq $0
    pushq $130
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_131
.type xul_x86_64_isr_131,@function
xul_x86_64_isr_131:
    pushq $0
    pushq $131
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_132
.type xul_x86_64_isr_132,@function
xul_x86_64_isr_132:
    pushq $0
    pushq $132
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_133
.type xul_x86_64_isr_133,@function
xul_x86_64_isr_133:
    pushq $0
    pushq $133
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_134
.type xul_x86_64_isr_134,@function
xul_x86_64_isr_134:
    pushq $0
    pushq $134
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_135
.type xul_x86_64_isr_135,@function
xul_x86_64_isr_135:
    pushq $0
    pushq $135
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_136
.type xul_x86_64_isr_136,@function
xul_x86_64_isr_136:
    pushq $0
    pushq $136
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_137
.type xul_x86_64_isr_137,@function
xul_x86_64_isr_137:
    pushq $0
    pushq $137
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_138
.type xul_x86_64_isr_138,@function
xul_x86_64_isr_138:
    pushq $0
    pushq $138
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_139
.type xul_x86_64_isr_139,@function
xul_x86_64_isr_139:
    pushq $0
    pushq $139
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_140
.type xul_x86_64_isr_140,@function
xul_x86_64_isr_140:
    pushq $0
    pushq $140
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_141
.type xul_x86_64_isr_141,@function
xul_x86_64_isr_141:
    pushq $0
    pushq $141
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_142
.type xul_x86_64_isr_142,@function
xul_x86_64_isr_142:
    pushq $0
    pushq $142
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_143
.type xul_x86_64_isr_143,@function
xul_x86_64_isr_143:
    pushq $0
    pushq $143
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_144
.type xul_x86_64_isr_144,@function
xul_x86_64_isr_144:
    pushq $0
    pushq $144
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_145
.type xul_x86_64_isr_145,@function
xul_x86_64_isr_145:
    pushq $0
    pushq $145
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_146
.type xul_x86_64_isr_146,@function
xul_x86_64_isr_146:
    pushq $0
    pushq $146
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_147
.type xul_x86_64_isr_147,@function
xul_x86_64_isr_147:
    pushq $0
    pushq $147
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_148
.type xul_x86_64_isr_148,@function
xul_x86_64_isr_148:
    pushq $0
    pushq $148
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_149
.type xul_x86_64_isr_149,@function
xul_x86_64_isr_149:
    pushq $0
    pushq $149
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_150
.type xul_x86_64_isr_150,@function
xul_x86_64_isr_150:
    pushq $0
    pushq $150
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_151
.type xul_x86_64_isr_151,@function
xul_x86_64_isr_151:
    pushq $0
    pushq $151
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_152
.type xul_x86_64_isr_152,@function
xul_x86_64_isr_152:
    pushq $0
    pushq $152
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_153
.type xul_x86_64_isr_153,@function
xul_x86_64_isr_153:
    pushq $0
    pushq $153
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_154
.type xul_x86_64_isr_154,@function
xul_x86_64_isr_154:
    pushq $0
    pushq $154
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_155
.type xul_x86_64_isr_155,@function
xul_x86_64_isr_155:
    pushq $0
    pushq $155
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_156
.type xul_x86_64_isr_156,@function
xul_x86_64_isr_156:
    pushq $0
    pushq $156
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_157
.type xul_x86_64_isr_157,@function
xul_x86_64_isr_157:
    pushq $0
    pushq $157
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_158
.type xul_x86_64_isr_158,@function
xul_x86_64_isr_158:
    pushq $0
    pushq $158
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_159
.type xul_x86_64_isr_159,@function
xul_x86_64_isr_159:
    pushq $0
    pushq $159
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_160
.type xul_x86_64_isr_160,@function
xul_x86_64_isr_160:
    pushq $0
    pushq $160
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_161
.type xul_x86_64_isr_161,@function
xul_x86_64_isr_161:
    pushq $0
    pushq $161
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_162
.type xul_x86_64_isr_162,@function
xul_x86_64_isr_162:
    pushq $0
    pushq $162
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_163
.type xul_x86_64_isr_163,@function
xul_x86_64_isr_163:
    pushq $0
    pushq $163
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_164
.type xul_x86_64_isr_164,@function
xul_x86_64_isr_164:
    pushq $0
    pushq $164
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_165
.type xul_x86_64_isr_165,@function
xul_x86_64_isr_165:
    pushq $0
    pushq $165
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_166
.type xul_x86_64_isr_166,@function
xul_x86_64_isr_166:
    pushq $0
    pushq $166
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_167
.type xul_x86_64_isr_167,@function
xul_x86_64_isr_167:
    pushq $0
    pushq $167
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_168
.type xul_x86_64_isr_168,@function
xul_x86_64_isr_168:
    pushq $0
    pushq $168
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_169
.type xul_x86_64_isr_169,@function
xul_x86_64_isr_169:
    pushq $0
    pushq $169
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_170
.type xul_x86_64_isr_170,@function
xul_x86_64_isr_170:
    pushq $0
    pushq $170
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_171
.type xul_x86_64_isr_171,@function
xul_x86_64_isr_171:
    pushq $0
    pushq $171
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_172
.type xul_x86_64_isr_172,@function
xul_x86_64_isr_172:
    pushq $0
    pushq $172
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_173
.type xul_x86_64_isr_173,@function
xul_x86_64_isr_173:
    pushq $0
    pushq $173
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_174
.type xul_x86_64_isr_174,@function
xul_x86_64_isr_174:
    pushq $0
    pushq $174
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_175
.type xul_x86_64_isr_175,@function
xul_x86_64_isr_175:
    pushq $0
    pushq $175
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_176
.type xul_x86_64_isr_176,@function
xul_x86_64_isr_176:
    pushq $0
    pushq $176
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_177
.type xul_x86_64_isr_177,@function
xul_x86_64_isr_177:
    pushq $0
    pushq $177
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_178
.type xul_x86_64_isr_178,@function
xul_x86_64_isr_178:
    pushq $0
    pushq $178
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_179
.type xul_x86_64_isr_179,@function
xul_x86_64_isr_179:
    pushq $0
    pushq $179
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_180
.type xul_x86_64_isr_180,@function
xul_x86_64_isr_180:
    pushq $0
    pushq $180
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_181
.type xul_x86_64_isr_181,@function
xul_x86_64_isr_181:
    pushq $0
    pushq $181
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_182
.type xul_x86_64_isr_182,@function
xul_x86_64_isr_182:
    pushq $0
    pushq $182
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_183
.type xul_x86_64_isr_183,@function
xul_x86_64_isr_183:
    pushq $0
    pushq $183
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_184
.type xul_x86_64_isr_184,@function
xul_x86_64_isr_184:
    pushq $0
    pushq $184
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_185
.type xul_x86_64_isr_185,@function
xul_x86_64_isr_185:
    pushq $0
    pushq $185
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_186
.type xul_x86_64_isr_186,@function
xul_x86_64_isr_186:
    pushq $0
    pushq $186
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_187
.type xul_x86_64_isr_187,@function
xul_x86_64_isr_187:
    pushq $0
    pushq $187
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_188
.type xul_x86_64_isr_188,@function
xul_x86_64_isr_188:
    pushq $0
    pushq $188
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_189
.type xul_x86_64_isr_189,@function
xul_x86_64_isr_189:
    pushq $0
    pushq $189
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_190
.type xul_x86_64_isr_190,@function
xul_x86_64_isr_190:
    pushq $0
    pushq $190
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_191
.type xul_x86_64_isr_191,@function
xul_x86_64_isr_191:
    pushq $0
    pushq $191
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_192
.type xul_x86_64_isr_192,@function
xul_x86_64_isr_192:
    pushq $0
    pushq $192
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_193
.type xul_x86_64_isr_193,@function
xul_x86_64_isr_193:
    pushq $0
    pushq $193
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_194
.type xul_x86_64_isr_194,@function
xul_x86_64_isr_194:
    pushq $0
    pushq $194
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_195
.type xul_x86_64_isr_195,@function
xul_x86_64_isr_195:
    pushq $0
    pushq $195
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_196
.type xul_x86_64_isr_196,@function
xul_x86_64_isr_196:
    pushq $0
    pushq $196
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_197
.type xul_x86_64_isr_197,@function
xul_x86_64_isr_197:
    pushq $0
    pushq $197
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_198
.type xul_x86_64_isr_198,@function
xul_x86_64_isr_198:
    pushq $0
    pushq $198
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_199
.type xul_x86_64_isr_199,@function
xul_x86_64_isr_199:
    pushq $0
    pushq $199
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_200
.type xul_x86_64_isr_200,@function
xul_x86_64_isr_200:
    pushq $0
    pushq $200
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_201
.type xul_x86_64_isr_201,@function
xul_x86_64_isr_201:
    pushq $0
    pushq $201
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_202
.type xul_x86_64_isr_202,@function
xul_x86_64_isr_202:
    pushq $0
    pushq $202
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_203
.type xul_x86_64_isr_203,@function
xul_x86_64_isr_203:
    pushq $0
    pushq $203
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_204
.type xul_x86_64_isr_204,@function
xul_x86_64_isr_204:
    pushq $0
    pushq $204
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_205
.type xul_x86_64_isr_205,@function
xul_x86_64_isr_205:
    pushq $0
    pushq $205
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_206
.type xul_x86_64_isr_206,@function
xul_x86_64_isr_206:
    pushq $0
    pushq $206
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_207
.type xul_x86_64_isr_207,@function
xul_x86_64_isr_207:
    pushq $0
    pushq $207
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_208
.type xul_x86_64_isr_208,@function
xul_x86_64_isr_208:
    pushq $0
    pushq $208
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_209
.type xul_x86_64_isr_209,@function
xul_x86_64_isr_209:
    pushq $0
    pushq $209
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_210
.type xul_x86_64_isr_210,@function
xul_x86_64_isr_210:
    pushq $0
    pushq $210
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_211
.type xul_x86_64_isr_211,@function
xul_x86_64_isr_211:
    pushq $0
    pushq $211
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_212
.type xul_x86_64_isr_212,@function
xul_x86_64_isr_212:
    pushq $0
    pushq $212
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_213
.type xul_x86_64_isr_213,@function
xul_x86_64_isr_213:
    pushq $0
    pushq $213
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_214
.type xul_x86_64_isr_214,@function
xul_x86_64_isr_214:
    pushq $0
    pushq $214
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_215
.type xul_x86_64_isr_215,@function
xul_x86_64_isr_215:
    pushq $0
    pushq $215
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_216
.type xul_x86_64_isr_216,@function
xul_x86_64_isr_216:
    pushq $0
    pushq $216
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_217
.type xul_x86_64_isr_217,@function
xul_x86_64_isr_217:
    pushq $0
    pushq $217
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_218
.type xul_x86_64_isr_218,@function
xul_x86_64_isr_218:
    pushq $0
    pushq $218
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_219
.type xul_x86_64_isr_219,@function
xul_x86_64_isr_219:
    pushq $0
    pushq $219
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_220
.type xul_x86_64_isr_220,@function
xul_x86_64_isr_220:
    pushq $0
    pushq $220
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_221
.type xul_x86_64_isr_221,@function
xul_x86_64_isr_221:
    pushq $0
    pushq $221
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_222
.type xul_x86_64_isr_222,@function
xul_x86_64_isr_222:
    pushq $0
    pushq $222
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_223
.type xul_x86_64_isr_223,@function
xul_x86_64_isr_223:
    pushq $0
    pushq $223
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_224
.type xul_x86_64_isr_224,@function
xul_x86_64_isr_224:
    pushq $0
    pushq $224
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_225
.type xul_x86_64_isr_225,@function
xul_x86_64_isr_225:
    pushq $0
    pushq $225
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_226
.type xul_x86_64_isr_226,@function
xul_x86_64_isr_226:
    pushq $0
    pushq $226
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_227
.type xul_x86_64_isr_227,@function
xul_x86_64_isr_227:
    pushq $0
    pushq $227
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_228
.type xul_x86_64_isr_228,@function
xul_x86_64_isr_228:
    pushq $0
    pushq $228
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_229
.type xul_x86_64_isr_229,@function
xul_x86_64_isr_229:
    pushq $0
    pushq $229
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_230
.type xul_x86_64_isr_230,@function
xul_x86_64_isr_230:
    pushq $0
    pushq $230
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_231
.type xul_x86_64_isr_231,@function
xul_x86_64_isr_231:
    pushq $0
    pushq $231
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_232
.type xul_x86_64_isr_232,@function
xul_x86_64_isr_232:
    pushq $0
    pushq $232
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_233
.type xul_x86_64_isr_233,@function
xul_x86_64_isr_233:
    pushq $0
    pushq $233
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_234
.type xul_x86_64_isr_234,@function
xul_x86_64_isr_234:
    pushq $0
    pushq $234
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_235
.type xul_x86_64_isr_235,@function
xul_x86_64_isr_235:
    pushq $0
    pushq $235
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_236
.type xul_x86_64_isr_236,@function
xul_x86_64_isr_236:
    pushq $0
    pushq $236
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_237
.type xul_x86_64_isr_237,@function
xul_x86_64_isr_237:
    pushq $0
    pushq $237
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_238
.type xul_x86_64_isr_238,@function
xul_x86_64_isr_238:
    pushq $0
    pushq $238
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_239
.type xul_x86_64_isr_239,@function
xul_x86_64_isr_239:
    pushq $0
    pushq $239
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_240
.type xul_x86_64_isr_240,@function
xul_x86_64_isr_240:
    pushq $0
    pushq $240
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_241
.type xul_x86_64_isr_241,@function
xul_x86_64_isr_241:
    pushq $0
    pushq $241
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_242
.type xul_x86_64_isr_242,@function
xul_x86_64_isr_242:
    pushq $0
    pushq $242
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_243
.type xul_x86_64_isr_243,@function
xul_x86_64_isr_243:
    pushq $0
    pushq $243
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_244
.type xul_x86_64_isr_244,@function
xul_x86_64_isr_244:
    pushq $0
    pushq $244
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_245
.type xul_x86_64_isr_245,@function
xul_x86_64_isr_245:
    pushq $0
    pushq $245
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_246
.type xul_x86_64_isr_246,@function
xul_x86_64_isr_246:
    pushq $0
    pushq $246
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_247
.type xul_x86_64_isr_247,@function
xul_x86_64_isr_247:
    pushq $0
    pushq $247
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_248
.type xul_x86_64_isr_248,@function
xul_x86_64_isr_248:
    pushq $0
    pushq $248
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_249
.type xul_x86_64_isr_249,@function
xul_x86_64_isr_249:
    pushq $0
    pushq $249
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_250
.type xul_x86_64_isr_250,@function
xul_x86_64_isr_250:
    pushq $0
    pushq $250
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_251
.type xul_x86_64_isr_251,@function
xul_x86_64_isr_251:
    pushq $0
    pushq $251
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_252
.type xul_x86_64_isr_252,@function
xul_x86_64_isr_252:
    pushq $0
    pushq $252
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_253
.type xul_x86_64_isr_253,@function
xul_x86_64_isr_253:
    pushq $0
    pushq $253
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_254
.type xul_x86_64_isr_254,@function
xul_x86_64_isr_254:
    pushq $0
    pushq $254
    jmp xul_x86_64_isr_common
.global xul_x86_64_isr_255
.type xul_x86_64_isr_255,@function
xul_x86_64_isr_255:
    pushq $0
    pushq $255
    jmp xul_x86_64_isr_common

.global xul_x86_64_isr_common
.type xul_x86_64_isr_common,@function
xul_x86_64_isr_common:
    cli
    cld
    pushq %rax
    pushq %rbx
    pushq %rcx
    pushq %rdx
    pushq %rbp
    pushq %rsi
    pushq %rdi
    pushq %r8
    pushq %r9
    pushq %r10
    pushq %r11
    pushq %r12
    pushq %r13
    pushq %r14
    pushq %r15

    /* Preserve the normalized frame while aligning the C call stack. */
    movq %rsp, %r12
    movq 120(%r12), %rdi
    movq 128(%r12), %rsi
    andq $-16, %rsp
    call xul_interrupt_dispatch
    movq %r12, %rsp
    popq %r15
    popq %r14
    popq %r13
    popq %r12
    popq %r11
    popq %r10
    popq %r9
    popq %r8
    popq %rdi
    popq %rsi
    popq %rbp
    popq %rdx
    popq %rcx
    popq %rbx
    popq %rax
    addq $16, %rsp
    iretq
.size xul_x86_64_isr_common, .-xul_x86_64_isr_common

.section .rodata.exceptions,"a",@progbits
.align 8
.global xul_x86_64_isr_stub_table
xul_x86_64_isr_stub_table:
    .quad xul_x86_64_isr_0
    .quad xul_x86_64_isr_1
    .quad xul_x86_64_isr_2
    .quad xul_x86_64_isr_3
    .quad xul_x86_64_isr_4
    .quad xul_x86_64_isr_5
    .quad xul_x86_64_isr_6
    .quad xul_x86_64_isr_7
    .quad xul_x86_64_isr_8
    .quad xul_x86_64_isr_9
    .quad xul_x86_64_isr_10
    .quad xul_x86_64_isr_11
    .quad xul_x86_64_isr_12
    .quad xul_x86_64_isr_13
    .quad xul_x86_64_isr_14
    .quad xul_x86_64_isr_15
    .quad xul_x86_64_isr_16
    .quad xul_x86_64_isr_17
    .quad xul_x86_64_isr_18
    .quad xul_x86_64_isr_19
    .quad xul_x86_64_isr_20
    .quad xul_x86_64_isr_21
    .quad xul_x86_64_isr_22
    .quad xul_x86_64_isr_23
    .quad xul_x86_64_isr_24
    .quad xul_x86_64_isr_25
    .quad xul_x86_64_isr_26
    .quad xul_x86_64_isr_27
    .quad xul_x86_64_isr_28
    .quad xul_x86_64_isr_29
    .quad xul_x86_64_isr_30
    .quad xul_x86_64_isr_31
    .quad xul_x86_64_isr_32
    .quad xul_x86_64_isr_33
    .quad xul_x86_64_isr_34
    .quad xul_x86_64_isr_35
    .quad xul_x86_64_isr_36
    .quad xul_x86_64_isr_37
    .quad xul_x86_64_isr_38
    .quad xul_x86_64_isr_39
    .quad xul_x86_64_isr_40
    .quad xul_x86_64_isr_41
    .quad xul_x86_64_isr_42
    .quad xul_x86_64_isr_43
    .quad xul_x86_64_isr_44
    .quad xul_x86_64_isr_45
    .quad xul_x86_64_isr_46
    .quad xul_x86_64_isr_47
    .quad xul_x86_64_isr_48
    .quad xul_x86_64_isr_49
    .quad xul_x86_64_isr_50
    .quad xul_x86_64_isr_51
    .quad xul_x86_64_isr_52
    .quad xul_x86_64_isr_53
    .quad xul_x86_64_isr_54
    .quad xul_x86_64_isr_55
    .quad xul_x86_64_isr_56
    .quad xul_x86_64_isr_57
    .quad xul_x86_64_isr_58
    .quad xul_x86_64_isr_59
    .quad xul_x86_64_isr_60
    .quad xul_x86_64_isr_61
    .quad xul_x86_64_isr_62
    .quad xul_x86_64_isr_63
    .quad xul_x86_64_isr_64
    .quad xul_x86_64_isr_65
    .quad xul_x86_64_isr_66
    .quad xul_x86_64_isr_67
    .quad xul_x86_64_isr_68
    .quad xul_x86_64_isr_69
    .quad xul_x86_64_isr_70
    .quad xul_x86_64_isr_71
    .quad xul_x86_64_isr_72
    .quad xul_x86_64_isr_73
    .quad xul_x86_64_isr_74
    .quad xul_x86_64_isr_75
    .quad xul_x86_64_isr_76
    .quad xul_x86_64_isr_77
    .quad xul_x86_64_isr_78
    .quad xul_x86_64_isr_79
    .quad xul_x86_64_isr_80
    .quad xul_x86_64_isr_81
    .quad xul_x86_64_isr_82
    .quad xul_x86_64_isr_83
    .quad xul_x86_64_isr_84
    .quad xul_x86_64_isr_85
    .quad xul_x86_64_isr_86
    .quad xul_x86_64_isr_87
    .quad xul_x86_64_isr_88
    .quad xul_x86_64_isr_89
    .quad xul_x86_64_isr_90
    .quad xul_x86_64_isr_91
    .quad xul_x86_64_isr_92
    .quad xul_x86_64_isr_93
    .quad xul_x86_64_isr_94
    .quad xul_x86_64_isr_95
    .quad xul_x86_64_isr_96
    .quad xul_x86_64_isr_97
    .quad xul_x86_64_isr_98
    .quad xul_x86_64_isr_99
    .quad xul_x86_64_isr_100
    .quad xul_x86_64_isr_101
    .quad xul_x86_64_isr_102
    .quad xul_x86_64_isr_103
    .quad xul_x86_64_isr_104
    .quad xul_x86_64_isr_105
    .quad xul_x86_64_isr_106
    .quad xul_x86_64_isr_107
    .quad xul_x86_64_isr_108
    .quad xul_x86_64_isr_109
    .quad xul_x86_64_isr_110
    .quad xul_x86_64_isr_111
    .quad xul_x86_64_isr_112
    .quad xul_x86_64_isr_113
    .quad xul_x86_64_isr_114
    .quad xul_x86_64_isr_115
    .quad xul_x86_64_isr_116
    .quad xul_x86_64_isr_117
    .quad xul_x86_64_isr_118
    .quad xul_x86_64_isr_119
    .quad xul_x86_64_isr_120
    .quad xul_x86_64_isr_121
    .quad xul_x86_64_isr_122
    .quad xul_x86_64_isr_123
    .quad xul_x86_64_isr_124
    .quad xul_x86_64_isr_125
    .quad xul_x86_64_isr_126
    .quad xul_x86_64_isr_127
    .quad xul_x86_64_isr_128
    .quad xul_x86_64_isr_129
    .quad xul_x86_64_isr_130
    .quad xul_x86_64_isr_131
    .quad xul_x86_64_isr_132
    .quad xul_x86_64_isr_133
    .quad xul_x86_64_isr_134
    .quad xul_x86_64_isr_135
    .quad xul_x86_64_isr_136
    .quad xul_x86_64_isr_137
    .quad xul_x86_64_isr_138
    .quad xul_x86_64_isr_139
    .quad xul_x86_64_isr_140
    .quad xul_x86_64_isr_141
    .quad xul_x86_64_isr_142
    .quad xul_x86_64_isr_143
    .quad xul_x86_64_isr_144
    .quad xul_x86_64_isr_145
    .quad xul_x86_64_isr_146
    .quad xul_x86_64_isr_147
    .quad xul_x86_64_isr_148
    .quad xul_x86_64_isr_149
    .quad xul_x86_64_isr_150
    .quad xul_x86_64_isr_151
    .quad xul_x86_64_isr_152
    .quad xul_x86_64_isr_153
    .quad xul_x86_64_isr_154
    .quad xul_x86_64_isr_155
    .quad xul_x86_64_isr_156
    .quad xul_x86_64_isr_157
    .quad xul_x86_64_isr_158
    .quad xul_x86_64_isr_159
    .quad xul_x86_64_isr_160
    .quad xul_x86_64_isr_161
    .quad xul_x86_64_isr_162
    .quad xul_x86_64_isr_163
    .quad xul_x86_64_isr_164
    .quad xul_x86_64_isr_165
    .quad xul_x86_64_isr_166
    .quad xul_x86_64_isr_167
    .quad xul_x86_64_isr_168
    .quad xul_x86_64_isr_169
    .quad xul_x86_64_isr_170
    .quad xul_x86_64_isr_171
    .quad xul_x86_64_isr_172
    .quad xul_x86_64_isr_173
    .quad xul_x86_64_isr_174
    .quad xul_x86_64_isr_175
    .quad xul_x86_64_isr_176
    .quad xul_x86_64_isr_177
    .quad xul_x86_64_isr_178
    .quad xul_x86_64_isr_179
    .quad xul_x86_64_isr_180
    .quad xul_x86_64_isr_181
    .quad xul_x86_64_isr_182
    .quad xul_x86_64_isr_183
    .quad xul_x86_64_isr_184
    .quad xul_x86_64_isr_185
    .quad xul_x86_64_isr_186
    .quad xul_x86_64_isr_187
    .quad xul_x86_64_isr_188
    .quad xul_x86_64_isr_189
    .quad xul_x86_64_isr_190
    .quad xul_x86_64_isr_191
    .quad xul_x86_64_isr_192
    .quad xul_x86_64_isr_193
    .quad xul_x86_64_isr_194
    .quad xul_x86_64_isr_195
    .quad xul_x86_64_isr_196
    .quad xul_x86_64_isr_197
    .quad xul_x86_64_isr_198
    .quad xul_x86_64_isr_199
    .quad xul_x86_64_isr_200
    .quad xul_x86_64_isr_201
    .quad xul_x86_64_isr_202
    .quad xul_x86_64_isr_203
    .quad xul_x86_64_isr_204
    .quad xul_x86_64_isr_205
    .quad xul_x86_64_isr_206
    .quad xul_x86_64_isr_207
    .quad xul_x86_64_isr_208
    .quad xul_x86_64_isr_209
    .quad xul_x86_64_isr_210
    .quad xul_x86_64_isr_211
    .quad xul_x86_64_isr_212
    .quad xul_x86_64_isr_213
    .quad xul_x86_64_isr_214
    .quad xul_x86_64_isr_215
    .quad xul_x86_64_isr_216
    .quad xul_x86_64_isr_217
    .quad xul_x86_64_isr_218
    .quad xul_x86_64_isr_219
    .quad xul_x86_64_isr_220
    .quad xul_x86_64_isr_221
    .quad xul_x86_64_isr_222
    .quad xul_x86_64_isr_223
    .quad xul_x86_64_isr_224
    .quad xul_x86_64_isr_225
    .quad xul_x86_64_isr_226
    .quad xul_x86_64_isr_227
    .quad xul_x86_64_isr_228
    .quad xul_x86_64_isr_229
    .quad xul_x86_64_isr_230
    .quad xul_x86_64_isr_231
    .quad xul_x86_64_isr_232
    .quad xul_x86_64_isr_233
    .quad xul_x86_64_isr_234
    .quad xul_x86_64_isr_235
    .quad xul_x86_64_isr_236
    .quad xul_x86_64_isr_237
    .quad xul_x86_64_isr_238
    .quad xul_x86_64_isr_239
    .quad xul_x86_64_isr_240
    .quad xul_x86_64_isr_241
    .quad xul_x86_64_isr_242
    .quad xul_x86_64_isr_243
    .quad xul_x86_64_isr_244
    .quad xul_x86_64_isr_245
    .quad xul_x86_64_isr_246
    .quad xul_x86_64_isr_247
    .quad xul_x86_64_isr_248
    .quad xul_x86_64_isr_249
    .quad xul_x86_64_isr_250
    .quad xul_x86_64_isr_251
    .quad xul_x86_64_isr_252
    .quad xul_x86_64_isr_253
    .quad xul_x86_64_isr_254
    .quad xul_x86_64_isr_255

.section .note.GNU-stack,"",@progbits
