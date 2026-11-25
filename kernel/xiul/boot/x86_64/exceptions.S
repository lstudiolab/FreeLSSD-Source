/*
 * This file has been modified by L,Studio.
 * Copyright (c) 2026 L,Studio. All rights reserved.
 *
 * Complete 256-entry IDT stub table. Unhandled vectors fail-stop until
 * architecture-specific exception and interrupt dispatch is installed.
 */
.section .text.exceptions,"ax",@progbits
.code64
.extern xiul_exception_fatal
.global xiul_x86_64_isr_0
.type xiul_x86_64_isr_0,@function
xiul_x86_64_isr_0:
    pushq $0
    pushq $0
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_1
.type xiul_x86_64_isr_1,@function
xiul_x86_64_isr_1:
    pushq $0
    pushq $1
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_2
.type xiul_x86_64_isr_2,@function
xiul_x86_64_isr_2:
    pushq $0
    pushq $2
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_3
.type xiul_x86_64_isr_3,@function
xiul_x86_64_isr_3:
    pushq $0
    pushq $3
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_4
.type xiul_x86_64_isr_4,@function
xiul_x86_64_isr_4:
    pushq $0
    pushq $4
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_5
.type xiul_x86_64_isr_5,@function
xiul_x86_64_isr_5:
    pushq $0
    pushq $5
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_6
.type xiul_x86_64_isr_6,@function
xiul_x86_64_isr_6:
    pushq $0
    pushq $6
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_7
.type xiul_x86_64_isr_7,@function
xiul_x86_64_isr_7:
    pushq $0
    pushq $7
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_8
.type xiul_x86_64_isr_8,@function
xiul_x86_64_isr_8:
    pushq $8
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_9
.type xiul_x86_64_isr_9,@function
xiul_x86_64_isr_9:
    pushq $0
    pushq $9
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_10
.type xiul_x86_64_isr_10,@function
xiul_x86_64_isr_10:
    pushq $10
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_11
.type xiul_x86_64_isr_11,@function
xiul_x86_64_isr_11:
    pushq $11
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_12
.type xiul_x86_64_isr_12,@function
xiul_x86_64_isr_12:
    pushq $12
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_13
.type xiul_x86_64_isr_13,@function
xiul_x86_64_isr_13:
    pushq $13
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_14
.type xiul_x86_64_isr_14,@function
xiul_x86_64_isr_14:
    pushq $14
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_15
.type xiul_x86_64_isr_15,@function
xiul_x86_64_isr_15:
    pushq $0
    pushq $15
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_16
.type xiul_x86_64_isr_16,@function
xiul_x86_64_isr_16:
    pushq $0
    pushq $16
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_17
.type xiul_x86_64_isr_17,@function
xiul_x86_64_isr_17:
    pushq $17
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_18
.type xiul_x86_64_isr_18,@function
xiul_x86_64_isr_18:
    pushq $0
    pushq $18
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_19
.type xiul_x86_64_isr_19,@function
xiul_x86_64_isr_19:
    pushq $0
    pushq $19
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_20
.type xiul_x86_64_isr_20,@function
xiul_x86_64_isr_20:
    pushq $0
    pushq $20
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_21
.type xiul_x86_64_isr_21,@function
xiul_x86_64_isr_21:
    pushq $21
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_22
.type xiul_x86_64_isr_22,@function
xiul_x86_64_isr_22:
    pushq $0
    pushq $22
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_23
.type xiul_x86_64_isr_23,@function
xiul_x86_64_isr_23:
    pushq $0
    pushq $23
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_24
.type xiul_x86_64_isr_24,@function
xiul_x86_64_isr_24:
    pushq $0
    pushq $24
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_25
.type xiul_x86_64_isr_25,@function
xiul_x86_64_isr_25:
    pushq $0
    pushq $25
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_26
.type xiul_x86_64_isr_26,@function
xiul_x86_64_isr_26:
    pushq $0
    pushq $26
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_27
.type xiul_x86_64_isr_27,@function
xiul_x86_64_isr_27:
    pushq $0
    pushq $27
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_28
.type xiul_x86_64_isr_28,@function
xiul_x86_64_isr_28:
    pushq $0
    pushq $28
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_29
.type xiul_x86_64_isr_29,@function
xiul_x86_64_isr_29:
    pushq $29
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_30
.type xiul_x86_64_isr_30,@function
xiul_x86_64_isr_30:
    pushq $30
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_31
.type xiul_x86_64_isr_31,@function
xiul_x86_64_isr_31:
    pushq $0
    pushq $31
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_32
.type xiul_x86_64_isr_32,@function
xiul_x86_64_isr_32:
    pushq $0
    pushq $32
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_33
.type xiul_x86_64_isr_33,@function
xiul_x86_64_isr_33:
    pushq $0
    pushq $33
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_34
.type xiul_x86_64_isr_34,@function
xiul_x86_64_isr_34:
    pushq $0
    pushq $34
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_35
.type xiul_x86_64_isr_35,@function
xiul_x86_64_isr_35:
    pushq $0
    pushq $35
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_36
.type xiul_x86_64_isr_36,@function
xiul_x86_64_isr_36:
    pushq $0
    pushq $36
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_37
.type xiul_x86_64_isr_37,@function
xiul_x86_64_isr_37:
    pushq $0
    pushq $37
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_38
.type xiul_x86_64_isr_38,@function
xiul_x86_64_isr_38:
    pushq $0
    pushq $38
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_39
.type xiul_x86_64_isr_39,@function
xiul_x86_64_isr_39:
    pushq $0
    pushq $39
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_40
.type xiul_x86_64_isr_40,@function
xiul_x86_64_isr_40:
    pushq $0
    pushq $40
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_41
.type xiul_x86_64_isr_41,@function
xiul_x86_64_isr_41:
    pushq $0
    pushq $41
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_42
.type xiul_x86_64_isr_42,@function
xiul_x86_64_isr_42:
    pushq $0
    pushq $42
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_43
.type xiul_x86_64_isr_43,@function
xiul_x86_64_isr_43:
    pushq $0
    pushq $43
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_44
.type xiul_x86_64_isr_44,@function
xiul_x86_64_isr_44:
    pushq $0
    pushq $44
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_45
.type xiul_x86_64_isr_45,@function
xiul_x86_64_isr_45:
    pushq $0
    pushq $45
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_46
.type xiul_x86_64_isr_46,@function
xiul_x86_64_isr_46:
    pushq $0
    pushq $46
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_47
.type xiul_x86_64_isr_47,@function
xiul_x86_64_isr_47:
    pushq $0
    pushq $47
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_48
.type xiul_x86_64_isr_48,@function
xiul_x86_64_isr_48:
    pushq $0
    pushq $48
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_49
.type xiul_x86_64_isr_49,@function
xiul_x86_64_isr_49:
    pushq $0
    pushq $49
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_50
.type xiul_x86_64_isr_50,@function
xiul_x86_64_isr_50:
    pushq $0
    pushq $50
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_51
.type xiul_x86_64_isr_51,@function
xiul_x86_64_isr_51:
    pushq $0
    pushq $51
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_52
.type xiul_x86_64_isr_52,@function
xiul_x86_64_isr_52:
    pushq $0
    pushq $52
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_53
.type xiul_x86_64_isr_53,@function
xiul_x86_64_isr_53:
    pushq $0
    pushq $53
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_54
.type xiul_x86_64_isr_54,@function
xiul_x86_64_isr_54:
    pushq $0
    pushq $54
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_55
.type xiul_x86_64_isr_55,@function
xiul_x86_64_isr_55:
    pushq $0
    pushq $55
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_56
.type xiul_x86_64_isr_56,@function
xiul_x86_64_isr_56:
    pushq $0
    pushq $56
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_57
.type xiul_x86_64_isr_57,@function
xiul_x86_64_isr_57:
    pushq $0
    pushq $57
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_58
.type xiul_x86_64_isr_58,@function
xiul_x86_64_isr_58:
    pushq $0
    pushq $58
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_59
.type xiul_x86_64_isr_59,@function
xiul_x86_64_isr_59:
    pushq $0
    pushq $59
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_60
.type xiul_x86_64_isr_60,@function
xiul_x86_64_isr_60:
    pushq $0
    pushq $60
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_61
.type xiul_x86_64_isr_61,@function
xiul_x86_64_isr_61:
    pushq $0
    pushq $61
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_62
.type xiul_x86_64_isr_62,@function
xiul_x86_64_isr_62:
    pushq $0
    pushq $62
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_63
.type xiul_x86_64_isr_63,@function
xiul_x86_64_isr_63:
    pushq $0
    pushq $63
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_64
.type xiul_x86_64_isr_64,@function
xiul_x86_64_isr_64:
    pushq $0
    pushq $64
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_65
.type xiul_x86_64_isr_65,@function
xiul_x86_64_isr_65:
    pushq $0
    pushq $65
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_66
.type xiul_x86_64_isr_66,@function
xiul_x86_64_isr_66:
    pushq $0
    pushq $66
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_67
.type xiul_x86_64_isr_67,@function
xiul_x86_64_isr_67:
    pushq $0
    pushq $67
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_68
.type xiul_x86_64_isr_68,@function
xiul_x86_64_isr_68:
    pushq $0
    pushq $68
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_69
.type xiul_x86_64_isr_69,@function
xiul_x86_64_isr_69:
    pushq $0
    pushq $69
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_70
.type xiul_x86_64_isr_70,@function
xiul_x86_64_isr_70:
    pushq $0
    pushq $70
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_71
.type xiul_x86_64_isr_71,@function
xiul_x86_64_isr_71:
    pushq $0
    pushq $71
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_72
.type xiul_x86_64_isr_72,@function
xiul_x86_64_isr_72:
    pushq $0
    pushq $72
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_73
.type xiul_x86_64_isr_73,@function
xiul_x86_64_isr_73:
    pushq $0
    pushq $73
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_74
.type xiul_x86_64_isr_74,@function
xiul_x86_64_isr_74:
    pushq $0
    pushq $74
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_75
.type xiul_x86_64_isr_75,@function
xiul_x86_64_isr_75:
    pushq $0
    pushq $75
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_76
.type xiul_x86_64_isr_76,@function
xiul_x86_64_isr_76:
    pushq $0
    pushq $76
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_77
.type xiul_x86_64_isr_77,@function
xiul_x86_64_isr_77:
    pushq $0
    pushq $77
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_78
.type xiul_x86_64_isr_78,@function
xiul_x86_64_isr_78:
    pushq $0
    pushq $78
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_79
.type xiul_x86_64_isr_79,@function
xiul_x86_64_isr_79:
    pushq $0
    pushq $79
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_80
.type xiul_x86_64_isr_80,@function
xiul_x86_64_isr_80:
    pushq $0
    pushq $80
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_81
.type xiul_x86_64_isr_81,@function
xiul_x86_64_isr_81:
    pushq $0
    pushq $81
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_82
.type xiul_x86_64_isr_82,@function
xiul_x86_64_isr_82:
    pushq $0
    pushq $82
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_83
.type xiul_x86_64_isr_83,@function
xiul_x86_64_isr_83:
    pushq $0
    pushq $83
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_84
.type xiul_x86_64_isr_84,@function
xiul_x86_64_isr_84:
    pushq $0
    pushq $84
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_85
.type xiul_x86_64_isr_85,@function
xiul_x86_64_isr_85:
    pushq $0
    pushq $85
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_86
.type xiul_x86_64_isr_86,@function
xiul_x86_64_isr_86:
    pushq $0
    pushq $86
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_87
.type xiul_x86_64_isr_87,@function
xiul_x86_64_isr_87:
    pushq $0
    pushq $87
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_88
.type xiul_x86_64_isr_88,@function
xiul_x86_64_isr_88:
    pushq $0
    pushq $88
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_89
.type xiul_x86_64_isr_89,@function
xiul_x86_64_isr_89:
    pushq $0
    pushq $89
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_90
.type xiul_x86_64_isr_90,@function
xiul_x86_64_isr_90:
    pushq $0
    pushq $90
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_91
.type xiul_x86_64_isr_91,@function
xiul_x86_64_isr_91:
    pushq $0
    pushq $91
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_92
.type xiul_x86_64_isr_92,@function
xiul_x86_64_isr_92:
    pushq $0
    pushq $92
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_93
.type xiul_x86_64_isr_93,@function
xiul_x86_64_isr_93:
    pushq $0
    pushq $93
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_94
.type xiul_x86_64_isr_94,@function
xiul_x86_64_isr_94:
    pushq $0
    pushq $94
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_95
.type xiul_x86_64_isr_95,@function
xiul_x86_64_isr_95:
    pushq $0
    pushq $95
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_96
.type xiul_x86_64_isr_96,@function
xiul_x86_64_isr_96:
    pushq $0
    pushq $96
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_97
.type xiul_x86_64_isr_97,@function
xiul_x86_64_isr_97:
    pushq $0
    pushq $97
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_98
.type xiul_x86_64_isr_98,@function
xiul_x86_64_isr_98:
    pushq $0
    pushq $98
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_99
.type xiul_x86_64_isr_99,@function
xiul_x86_64_isr_99:
    pushq $0
    pushq $99
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_100
.type xiul_x86_64_isr_100,@function
xiul_x86_64_isr_100:
    pushq $0
    pushq $100
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_101
.type xiul_x86_64_isr_101,@function
xiul_x86_64_isr_101:
    pushq $0
    pushq $101
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_102
.type xiul_x86_64_isr_102,@function
xiul_x86_64_isr_102:
    pushq $0
    pushq $102
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_103
.type xiul_x86_64_isr_103,@function
xiul_x86_64_isr_103:
    pushq $0
    pushq $103
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_104
.type xiul_x86_64_isr_104,@function
xiul_x86_64_isr_104:
    pushq $0
    pushq $104
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_105
.type xiul_x86_64_isr_105,@function
xiul_x86_64_isr_105:
    pushq $0
    pushq $105
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_106
.type xiul_x86_64_isr_106,@function
xiul_x86_64_isr_106:
    pushq $0
    pushq $106
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_107
.type xiul_x86_64_isr_107,@function
xiul_x86_64_isr_107:
    pushq $0
    pushq $107
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_108
.type xiul_x86_64_isr_108,@function
xiul_x86_64_isr_108:
    pushq $0
    pushq $108
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_109
.type xiul_x86_64_isr_109,@function
xiul_x86_64_isr_109:
    pushq $0
    pushq $109
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_110
.type xiul_x86_64_isr_110,@function
xiul_x86_64_isr_110:
    pushq $0
    pushq $110
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_111
.type xiul_x86_64_isr_111,@function
xiul_x86_64_isr_111:
    pushq $0
    pushq $111
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_112
.type xiul_x86_64_isr_112,@function
xiul_x86_64_isr_112:
    pushq $0
    pushq $112
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_113
.type xiul_x86_64_isr_113,@function
xiul_x86_64_isr_113:
    pushq $0
    pushq $113
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_114
.type xiul_x86_64_isr_114,@function
xiul_x86_64_isr_114:
    pushq $0
    pushq $114
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_115
.type xiul_x86_64_isr_115,@function
xiul_x86_64_isr_115:
    pushq $0
    pushq $115
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_116
.type xiul_x86_64_isr_116,@function
xiul_x86_64_isr_116:
    pushq $0
    pushq $116
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_117
.type xiul_x86_64_isr_117,@function
xiul_x86_64_isr_117:
    pushq $0
    pushq $117
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_118
.type xiul_x86_64_isr_118,@function
xiul_x86_64_isr_118:
    pushq $0
    pushq $118
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_119
.type xiul_x86_64_isr_119,@function
xiul_x86_64_isr_119:
    pushq $0
    pushq $119
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_120
.type xiul_x86_64_isr_120,@function
xiul_x86_64_isr_120:
    pushq $0
    pushq $120
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_121
.type xiul_x86_64_isr_121,@function
xiul_x86_64_isr_121:
    pushq $0
    pushq $121
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_122
.type xiul_x86_64_isr_122,@function
xiul_x86_64_isr_122:
    pushq $0
    pushq $122
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_123
.type xiul_x86_64_isr_123,@function
xiul_x86_64_isr_123:
    pushq $0
    pushq $123
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_124
.type xiul_x86_64_isr_124,@function
xiul_x86_64_isr_124:
    pushq $0
    pushq $124
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_125
.type xiul_x86_64_isr_125,@function
xiul_x86_64_isr_125:
    pushq $0
    pushq $125
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_126
.type xiul_x86_64_isr_126,@function
xiul_x86_64_isr_126:
    pushq $0
    pushq $126
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_127
.type xiul_x86_64_isr_127,@function
xiul_x86_64_isr_127:
    pushq $0
    pushq $127
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_128
.type xiul_x86_64_isr_128,@function
xiul_x86_64_isr_128:
    pushq $0
    pushq $128
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_129
.type xiul_x86_64_isr_129,@function
xiul_x86_64_isr_129:
    pushq $0
    pushq $129
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_130
.type xiul_x86_64_isr_130,@function
xiul_x86_64_isr_130:
    pushq $0
    pushq $130
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_131
.type xiul_x86_64_isr_131,@function
xiul_x86_64_isr_131:
    pushq $0
    pushq $131
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_132
.type xiul_x86_64_isr_132,@function
xiul_x86_64_isr_132:
    pushq $0
    pushq $132
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_133
.type xiul_x86_64_isr_133,@function
xiul_x86_64_isr_133:
    pushq $0
    pushq $133
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_134
.type xiul_x86_64_isr_134,@function
xiul_x86_64_isr_134:
    pushq $0
    pushq $134
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_135
.type xiul_x86_64_isr_135,@function
xiul_x86_64_isr_135:
    pushq $0
    pushq $135
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_136
.type xiul_x86_64_isr_136,@function
xiul_x86_64_isr_136:
    pushq $0
    pushq $136
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_137
.type xiul_x86_64_isr_137,@function
xiul_x86_64_isr_137:
    pushq $0
    pushq $137
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_138
.type xiul_x86_64_isr_138,@function
xiul_x86_64_isr_138:
    pushq $0
    pushq $138
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_139
.type xiul_x86_64_isr_139,@function
xiul_x86_64_isr_139:
    pushq $0
    pushq $139
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_140
.type xiul_x86_64_isr_140,@function
xiul_x86_64_isr_140:
    pushq $0
    pushq $140
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_141
.type xiul_x86_64_isr_141,@function
xiul_x86_64_isr_141:
    pushq $0
    pushq $141
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_142
.type xiul_x86_64_isr_142,@function
xiul_x86_64_isr_142:
    pushq $0
    pushq $142
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_143
.type xiul_x86_64_isr_143,@function
xiul_x86_64_isr_143:
    pushq $0
    pushq $143
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_144
.type xiul_x86_64_isr_144,@function
xiul_x86_64_isr_144:
    pushq $0
    pushq $144
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_145
.type xiul_x86_64_isr_145,@function
xiul_x86_64_isr_145:
    pushq $0
    pushq $145
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_146
.type xiul_x86_64_isr_146,@function
xiul_x86_64_isr_146:
    pushq $0
    pushq $146
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_147
.type xiul_x86_64_isr_147,@function
xiul_x86_64_isr_147:
    pushq $0
    pushq $147
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_148
.type xiul_x86_64_isr_148,@function
xiul_x86_64_isr_148:
    pushq $0
    pushq $148
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_149
.type xiul_x86_64_isr_149,@function
xiul_x86_64_isr_149:
    pushq $0
    pushq $149
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_150
.type xiul_x86_64_isr_150,@function
xiul_x86_64_isr_150:
    pushq $0
    pushq $150
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_151
.type xiul_x86_64_isr_151,@function
xiul_x86_64_isr_151:
    pushq $0
    pushq $151
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_152
.type xiul_x86_64_isr_152,@function
xiul_x86_64_isr_152:
    pushq $0
    pushq $152
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_153
.type xiul_x86_64_isr_153,@function
xiul_x86_64_isr_153:
    pushq $0
    pushq $153
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_154
.type xiul_x86_64_isr_154,@function
xiul_x86_64_isr_154:
    pushq $0
    pushq $154
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_155
.type xiul_x86_64_isr_155,@function
xiul_x86_64_isr_155:
    pushq $0
    pushq $155
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_156
.type xiul_x86_64_isr_156,@function
xiul_x86_64_isr_156:
    pushq $0
    pushq $156
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_157
.type xiul_x86_64_isr_157,@function
xiul_x86_64_isr_157:
    pushq $0
    pushq $157
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_158
.type xiul_x86_64_isr_158,@function
xiul_x86_64_isr_158:
    pushq $0
    pushq $158
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_159
.type xiul_x86_64_isr_159,@function
xiul_x86_64_isr_159:
    pushq $0
    pushq $159
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_160
.type xiul_x86_64_isr_160,@function
xiul_x86_64_isr_160:
    pushq $0
    pushq $160
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_161
.type xiul_x86_64_isr_161,@function
xiul_x86_64_isr_161:
    pushq $0
    pushq $161
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_162
.type xiul_x86_64_isr_162,@function
xiul_x86_64_isr_162:
    pushq $0
    pushq $162
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_163
.type xiul_x86_64_isr_163,@function
xiul_x86_64_isr_163:
    pushq $0
    pushq $163
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_164
.type xiul_x86_64_isr_164,@function
xiul_x86_64_isr_164:
    pushq $0
    pushq $164
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_165
.type xiul_x86_64_isr_165,@function
xiul_x86_64_isr_165:
    pushq $0
    pushq $165
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_166
.type xiul_x86_64_isr_166,@function
xiul_x86_64_isr_166:
    pushq $0
    pushq $166
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_167
.type xiul_x86_64_isr_167,@function
xiul_x86_64_isr_167:
    pushq $0
    pushq $167
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_168
.type xiul_x86_64_isr_168,@function
xiul_x86_64_isr_168:
    pushq $0
    pushq $168
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_169
.type xiul_x86_64_isr_169,@function
xiul_x86_64_isr_169:
    pushq $0
    pushq $169
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_170
.type xiul_x86_64_isr_170,@function
xiul_x86_64_isr_170:
    pushq $0
    pushq $170
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_171
.type xiul_x86_64_isr_171,@function
xiul_x86_64_isr_171:
    pushq $0
    pushq $171
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_172
.type xiul_x86_64_isr_172,@function
xiul_x86_64_isr_172:
    pushq $0
    pushq $172
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_173
.type xiul_x86_64_isr_173,@function
xiul_x86_64_isr_173:
    pushq $0
    pushq $173
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_174
.type xiul_x86_64_isr_174,@function
xiul_x86_64_isr_174:
    pushq $0
    pushq $174
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_175
.type xiul_x86_64_isr_175,@function
xiul_x86_64_isr_175:
    pushq $0
    pushq $175
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_176
.type xiul_x86_64_isr_176,@function
xiul_x86_64_isr_176:
    pushq $0
    pushq $176
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_177
.type xiul_x86_64_isr_177,@function
xiul_x86_64_isr_177:
    pushq $0
    pushq $177
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_178
.type xiul_x86_64_isr_178,@function
xiul_x86_64_isr_178:
    pushq $0
    pushq $178
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_179
.type xiul_x86_64_isr_179,@function
xiul_x86_64_isr_179:
    pushq $0
    pushq $179
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_180
.type xiul_x86_64_isr_180,@function
xiul_x86_64_isr_180:
    pushq $0
    pushq $180
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_181
.type xiul_x86_64_isr_181,@function
xiul_x86_64_isr_181:
    pushq $0
    pushq $181
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_182
.type xiul_x86_64_isr_182,@function
xiul_x86_64_isr_182:
    pushq $0
    pushq $182
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_183
.type xiul_x86_64_isr_183,@function
xiul_x86_64_isr_183:
    pushq $0
    pushq $183
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_184
.type xiul_x86_64_isr_184,@function
xiul_x86_64_isr_184:
    pushq $0
    pushq $184
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_185
.type xiul_x86_64_isr_185,@function
xiul_x86_64_isr_185:
    pushq $0
    pushq $185
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_186
.type xiul_x86_64_isr_186,@function
xiul_x86_64_isr_186:
    pushq $0
    pushq $186
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_187
.type xiul_x86_64_isr_187,@function
xiul_x86_64_isr_187:
    pushq $0
    pushq $187
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_188
.type xiul_x86_64_isr_188,@function
xiul_x86_64_isr_188:
    pushq $0
    pushq $188
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_189
.type xiul_x86_64_isr_189,@function
xiul_x86_64_isr_189:
    pushq $0
    pushq $189
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_190
.type xiul_x86_64_isr_190,@function
xiul_x86_64_isr_190:
    pushq $0
    pushq $190
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_191
.type xiul_x86_64_isr_191,@function
xiul_x86_64_isr_191:
    pushq $0
    pushq $191
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_192
.type xiul_x86_64_isr_192,@function
xiul_x86_64_isr_192:
    pushq $0
    pushq $192
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_193
.type xiul_x86_64_isr_193,@function
xiul_x86_64_isr_193:
    pushq $0
    pushq $193
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_194
.type xiul_x86_64_isr_194,@function
xiul_x86_64_isr_194:
    pushq $0
    pushq $194
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_195
.type xiul_x86_64_isr_195,@function
xiul_x86_64_isr_195:
    pushq $0
    pushq $195
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_196
.type xiul_x86_64_isr_196,@function
xiul_x86_64_isr_196:
    pushq $0
    pushq $196
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_197
.type xiul_x86_64_isr_197,@function
xiul_x86_64_isr_197:
    pushq $0
    pushq $197
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_198
.type xiul_x86_64_isr_198,@function
xiul_x86_64_isr_198:
    pushq $0
    pushq $198
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_199
.type xiul_x86_64_isr_199,@function
xiul_x86_64_isr_199:
    pushq $0
    pushq $199
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_200
.type xiul_x86_64_isr_200,@function
xiul_x86_64_isr_200:
    pushq $0
    pushq $200
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_201
.type xiul_x86_64_isr_201,@function
xiul_x86_64_isr_201:
    pushq $0
    pushq $201
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_202
.type xiul_x86_64_isr_202,@function
xiul_x86_64_isr_202:
    pushq $0
    pushq $202
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_203
.type xiul_x86_64_isr_203,@function
xiul_x86_64_isr_203:
    pushq $0
    pushq $203
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_204
.type xiul_x86_64_isr_204,@function
xiul_x86_64_isr_204:
    pushq $0
    pushq $204
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_205
.type xiul_x86_64_isr_205,@function
xiul_x86_64_isr_205:
    pushq $0
    pushq $205
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_206
.type xiul_x86_64_isr_206,@function
xiul_x86_64_isr_206:
    pushq $0
    pushq $206
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_207
.type xiul_x86_64_isr_207,@function
xiul_x86_64_isr_207:
    pushq $0
    pushq $207
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_208
.type xiul_x86_64_isr_208,@function
xiul_x86_64_isr_208:
    pushq $0
    pushq $208
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_209
.type xiul_x86_64_isr_209,@function
xiul_x86_64_isr_209:
    pushq $0
    pushq $209
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_210
.type xiul_x86_64_isr_210,@function
xiul_x86_64_isr_210:
    pushq $0
    pushq $210
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_211
.type xiul_x86_64_isr_211,@function
xiul_x86_64_isr_211:
    pushq $0
    pushq $211
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_212
.type xiul_x86_64_isr_212,@function
xiul_x86_64_isr_212:
    pushq $0
    pushq $212
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_213
.type xiul_x86_64_isr_213,@function
xiul_x86_64_isr_213:
    pushq $0
    pushq $213
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_214
.type xiul_x86_64_isr_214,@function
xiul_x86_64_isr_214:
    pushq $0
    pushq $214
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_215
.type xiul_x86_64_isr_215,@function
xiul_x86_64_isr_215:
    pushq $0
    pushq $215
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_216
.type xiul_x86_64_isr_216,@function
xiul_x86_64_isr_216:
    pushq $0
    pushq $216
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_217
.type xiul_x86_64_isr_217,@function
xiul_x86_64_isr_217:
    pushq $0
    pushq $217
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_218
.type xiul_x86_64_isr_218,@function
xiul_x86_64_isr_218:
    pushq $0
    pushq $218
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_219
.type xiul_x86_64_isr_219,@function
xiul_x86_64_isr_219:
    pushq $0
    pushq $219
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_220
.type xiul_x86_64_isr_220,@function
xiul_x86_64_isr_220:
    pushq $0
    pushq $220
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_221
.type xiul_x86_64_isr_221,@function
xiul_x86_64_isr_221:
    pushq $0
    pushq $221
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_222
.type xiul_x86_64_isr_222,@function
xiul_x86_64_isr_222:
    pushq $0
    pushq $222
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_223
.type xiul_x86_64_isr_223,@function
xiul_x86_64_isr_223:
    pushq $0
    pushq $223
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_224
.type xiul_x86_64_isr_224,@function
xiul_x86_64_isr_224:
    pushq $0
    pushq $224
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_225
.type xiul_x86_64_isr_225,@function
xiul_x86_64_isr_225:
    pushq $0
    pushq $225
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_226
.type xiul_x86_64_isr_226,@function
xiul_x86_64_isr_226:
    pushq $0
    pushq $226
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_227
.type xiul_x86_64_isr_227,@function
xiul_x86_64_isr_227:
    pushq $0
    pushq $227
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_228
.type xiul_x86_64_isr_228,@function
xiul_x86_64_isr_228:
    pushq $0
    pushq $228
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_229
.type xiul_x86_64_isr_229,@function
xiul_x86_64_isr_229:
    pushq $0
    pushq $229
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_230
.type xiul_x86_64_isr_230,@function
xiul_x86_64_isr_230:
    pushq $0
    pushq $230
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_231
.type xiul_x86_64_isr_231,@function
xiul_x86_64_isr_231:
    pushq $0
    pushq $231
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_232
.type xiul_x86_64_isr_232,@function
xiul_x86_64_isr_232:
    pushq $0
    pushq $232
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_233
.type xiul_x86_64_isr_233,@function
xiul_x86_64_isr_233:
    pushq $0
    pushq $233
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_234
.type xiul_x86_64_isr_234,@function
xiul_x86_64_isr_234:
    pushq $0
    pushq $234
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_235
.type xiul_x86_64_isr_235,@function
xiul_x86_64_isr_235:
    pushq $0
    pushq $235
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_236
.type xiul_x86_64_isr_236,@function
xiul_x86_64_isr_236:
    pushq $0
    pushq $236
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_237
.type xiul_x86_64_isr_237,@function
xiul_x86_64_isr_237:
    pushq $0
    pushq $237
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_238
.type xiul_x86_64_isr_238,@function
xiul_x86_64_isr_238:
    pushq $0
    pushq $238
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_239
.type xiul_x86_64_isr_239,@function
xiul_x86_64_isr_239:
    pushq $0
    pushq $239
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_240
.type xiul_x86_64_isr_240,@function
xiul_x86_64_isr_240:
    pushq $0
    pushq $240
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_241
.type xiul_x86_64_isr_241,@function
xiul_x86_64_isr_241:
    pushq $0
    pushq $241
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_242
.type xiul_x86_64_isr_242,@function
xiul_x86_64_isr_242:
    pushq $0
    pushq $242
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_243
.type xiul_x86_64_isr_243,@function
xiul_x86_64_isr_243:
    pushq $0
    pushq $243
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_244
.type xiul_x86_64_isr_244,@function
xiul_x86_64_isr_244:
    pushq $0
    pushq $244
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_245
.type xiul_x86_64_isr_245,@function
xiul_x86_64_isr_245:
    pushq $0
    pushq $245
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_246
.type xiul_x86_64_isr_246,@function
xiul_x86_64_isr_246:
    pushq $0
    pushq $246
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_247
.type xiul_x86_64_isr_247,@function
xiul_x86_64_isr_247:
    pushq $0
    pushq $247
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_248
.type xiul_x86_64_isr_248,@function
xiul_x86_64_isr_248:
    pushq $0
    pushq $248
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_249
.type xiul_x86_64_isr_249,@function
xiul_x86_64_isr_249:
    pushq $0
    pushq $249
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_250
.type xiul_x86_64_isr_250,@function
xiul_x86_64_isr_250:
    pushq $0
    pushq $250
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_251
.type xiul_x86_64_isr_251,@function
xiul_x86_64_isr_251:
    pushq $0
    pushq $251
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_252
.type xiul_x86_64_isr_252,@function
xiul_x86_64_isr_252:
    pushq $0
    pushq $252
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_253
.type xiul_x86_64_isr_253,@function
xiul_x86_64_isr_253:
    pushq $0
    pushq $253
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_254
.type xiul_x86_64_isr_254,@function
xiul_x86_64_isr_254:
    pushq $0
    pushq $254
    jmp xiul_x86_64_isr_common
.global xiul_x86_64_isr_255
.type xiul_x86_64_isr_255,@function
xiul_x86_64_isr_255:
    pushq $0
    pushq $255
    jmp xiul_x86_64_isr_common

.global xiul_x86_64_isr_common
.type xiul_x86_64_isr_common,@function
xiul_x86_64_isr_common:
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

    /* Normalized frame: vector at +120, CPU error code at +128. */
    movq 120(%rsp), %rdi
    movq 128(%rsp), %rsi
    andq $-16, %rsp
    call xiul_exception_dispatch
1:
    cli
    hlt
    jmp 1b
.size xiul_x86_64_isr_common, .-xiul_x86_64_isr_common

.section .rodata.exceptions,"a",@progbits
.align 8
.global xiul_x86_64_isr_stub_table
xiul_x86_64_isr_stub_table:
    .quad xiul_x86_64_isr_0
    .quad xiul_x86_64_isr_1
    .quad xiul_x86_64_isr_2
    .quad xiul_x86_64_isr_3
    .quad xiul_x86_64_isr_4
    .quad xiul_x86_64_isr_5
    .quad xiul_x86_64_isr_6
    .quad xiul_x86_64_isr_7
    .quad xiul_x86_64_isr_8
    .quad xiul_x86_64_isr_9
    .quad xiul_x86_64_isr_10
    .quad xiul_x86_64_isr_11
    .quad xiul_x86_64_isr_12
    .quad xiul_x86_64_isr_13
    .quad xiul_x86_64_isr_14
    .quad xiul_x86_64_isr_15
    .quad xiul_x86_64_isr_16
    .quad xiul_x86_64_isr_17
    .quad xiul_x86_64_isr_18
    .quad xiul_x86_64_isr_19
    .quad xiul_x86_64_isr_20
    .quad xiul_x86_64_isr_21
    .quad xiul_x86_64_isr_22
    .quad xiul_x86_64_isr_23
    .quad xiul_x86_64_isr_24
    .quad xiul_x86_64_isr_25
    .quad xiul_x86_64_isr_26
    .quad xiul_x86_64_isr_27
    .quad xiul_x86_64_isr_28
    .quad xiul_x86_64_isr_29
    .quad xiul_x86_64_isr_30
    .quad xiul_x86_64_isr_31
    .quad xiul_x86_64_isr_32
    .quad xiul_x86_64_isr_33
    .quad xiul_x86_64_isr_34
    .quad xiul_x86_64_isr_35
    .quad xiul_x86_64_isr_36
    .quad xiul_x86_64_isr_37
    .quad xiul_x86_64_isr_38
    .quad xiul_x86_64_isr_39
    .quad xiul_x86_64_isr_40
    .quad xiul_x86_64_isr_41
    .quad xiul_x86_64_isr_42
    .quad xiul_x86_64_isr_43
    .quad xiul_x86_64_isr_44
    .quad xiul_x86_64_isr_45
    .quad xiul_x86_64_isr_46
    .quad xiul_x86_64_isr_47
    .quad xiul_x86_64_isr_48
    .quad xiul_x86_64_isr_49
    .quad xiul_x86_64_isr_50
    .quad xiul_x86_64_isr_51
    .quad xiul_x86_64_isr_52
    .quad xiul_x86_64_isr_53
    .quad xiul_x86_64_isr_54
    .quad xiul_x86_64_isr_55
    .quad xiul_x86_64_isr_56
    .quad xiul_x86_64_isr_57
    .quad xiul_x86_64_isr_58
    .quad xiul_x86_64_isr_59
    .quad xiul_x86_64_isr_60
    .quad xiul_x86_64_isr_61
    .quad xiul_x86_64_isr_62
    .quad xiul_x86_64_isr_63
    .quad xiul_x86_64_isr_64
    .quad xiul_x86_64_isr_65
    .quad xiul_x86_64_isr_66
    .quad xiul_x86_64_isr_67
    .quad xiul_x86_64_isr_68
    .quad xiul_x86_64_isr_69
    .quad xiul_x86_64_isr_70
    .quad xiul_x86_64_isr_71
    .quad xiul_x86_64_isr_72
    .quad xiul_x86_64_isr_73
    .quad xiul_x86_64_isr_74
    .quad xiul_x86_64_isr_75
    .quad xiul_x86_64_isr_76
    .quad xiul_x86_64_isr_77
    .quad xiul_x86_64_isr_78
    .quad xiul_x86_64_isr_79
    .quad xiul_x86_64_isr_80
    .quad xiul_x86_64_isr_81
    .quad xiul_x86_64_isr_82
    .quad xiul_x86_64_isr_83
    .quad xiul_x86_64_isr_84
    .quad xiul_x86_64_isr_85
    .quad xiul_x86_64_isr_86
    .quad xiul_x86_64_isr_87
    .quad xiul_x86_64_isr_88
    .quad xiul_x86_64_isr_89
    .quad xiul_x86_64_isr_90
    .quad xiul_x86_64_isr_91
    .quad xiul_x86_64_isr_92
    .quad xiul_x86_64_isr_93
    .quad xiul_x86_64_isr_94
    .quad xiul_x86_64_isr_95
    .quad xiul_x86_64_isr_96
    .quad xiul_x86_64_isr_97
    .quad xiul_x86_64_isr_98
    .quad xiul_x86_64_isr_99
    .quad xiul_x86_64_isr_100
    .quad xiul_x86_64_isr_101
    .quad xiul_x86_64_isr_102
    .quad xiul_x86_64_isr_103
    .quad xiul_x86_64_isr_104
    .quad xiul_x86_64_isr_105
    .quad xiul_x86_64_isr_106
    .quad xiul_x86_64_isr_107
    .quad xiul_x86_64_isr_108
    .quad xiul_x86_64_isr_109
    .quad xiul_x86_64_isr_110
    .quad xiul_x86_64_isr_111
    .quad xiul_x86_64_isr_112
    .quad xiul_x86_64_isr_113
    .quad xiul_x86_64_isr_114
    .quad xiul_x86_64_isr_115
    .quad xiul_x86_64_isr_116
    .quad xiul_x86_64_isr_117
    .quad xiul_x86_64_isr_118
    .quad xiul_x86_64_isr_119
    .quad xiul_x86_64_isr_120
    .quad xiul_x86_64_isr_121
    .quad xiul_x86_64_isr_122
    .quad xiul_x86_64_isr_123
    .quad xiul_x86_64_isr_124
    .quad xiul_x86_64_isr_125
    .quad xiul_x86_64_isr_126
    .quad xiul_x86_64_isr_127
    .quad xiul_x86_64_isr_128
    .quad xiul_x86_64_isr_129
    .quad xiul_x86_64_isr_130
    .quad xiul_x86_64_isr_131
    .quad xiul_x86_64_isr_132
    .quad xiul_x86_64_isr_133
    .quad xiul_x86_64_isr_134
    .quad xiul_x86_64_isr_135
    .quad xiul_x86_64_isr_136
    .quad xiul_x86_64_isr_137
    .quad xiul_x86_64_isr_138
    .quad xiul_x86_64_isr_139
    .quad xiul_x86_64_isr_140
    .quad xiul_x86_64_isr_141
    .quad xiul_x86_64_isr_142
    .quad xiul_x86_64_isr_143
    .quad xiul_x86_64_isr_144
    .quad xiul_x86_64_isr_145
    .quad xiul_x86_64_isr_146
    .quad xiul_x86_64_isr_147
    .quad xiul_x86_64_isr_148
    .quad xiul_x86_64_isr_149
    .quad xiul_x86_64_isr_150
    .quad xiul_x86_64_isr_151
    .quad xiul_x86_64_isr_152
    .quad xiul_x86_64_isr_153
    .quad xiul_x86_64_isr_154
    .quad xiul_x86_64_isr_155
    .quad xiul_x86_64_isr_156
    .quad xiul_x86_64_isr_157
    .quad xiul_x86_64_isr_158
    .quad xiul_x86_64_isr_159
    .quad xiul_x86_64_isr_160
    .quad xiul_x86_64_isr_161
    .quad xiul_x86_64_isr_162
    .quad xiul_x86_64_isr_163
    .quad xiul_x86_64_isr_164
    .quad xiul_x86_64_isr_165
    .quad xiul_x86_64_isr_166
    .quad xiul_x86_64_isr_167
    .quad xiul_x86_64_isr_168
    .quad xiul_x86_64_isr_169
    .quad xiul_x86_64_isr_170
    .quad xiul_x86_64_isr_171
    .quad xiul_x86_64_isr_172
    .quad xiul_x86_64_isr_173
    .quad xiul_x86_64_isr_174
    .quad xiul_x86_64_isr_175
    .quad xiul_x86_64_isr_176
    .quad xiul_x86_64_isr_177
    .quad xiul_x86_64_isr_178
    .quad xiul_x86_64_isr_179
    .quad xiul_x86_64_isr_180
    .quad xiul_x86_64_isr_181
    .quad xiul_x86_64_isr_182
    .quad xiul_x86_64_isr_183
    .quad xiul_x86_64_isr_184
    .quad xiul_x86_64_isr_185
    .quad xiul_x86_64_isr_186
    .quad xiul_x86_64_isr_187
    .quad xiul_x86_64_isr_188
    .quad xiul_x86_64_isr_189
    .quad xiul_x86_64_isr_190
    .quad xiul_x86_64_isr_191
    .quad xiul_x86_64_isr_192
    .quad xiul_x86_64_isr_193
    .quad xiul_x86_64_isr_194
    .quad xiul_x86_64_isr_195
    .quad xiul_x86_64_isr_196
    .quad xiul_x86_64_isr_197
    .quad xiul_x86_64_isr_198
    .quad xiul_x86_64_isr_199
    .quad xiul_x86_64_isr_200
    .quad xiul_x86_64_isr_201
    .quad xiul_x86_64_isr_202
    .quad xiul_x86_64_isr_203
    .quad xiul_x86_64_isr_204
    .quad xiul_x86_64_isr_205
    .quad xiul_x86_64_isr_206
    .quad xiul_x86_64_isr_207
    .quad xiul_x86_64_isr_208
    .quad xiul_x86_64_isr_209
    .quad xiul_x86_64_isr_210
    .quad xiul_x86_64_isr_211
    .quad xiul_x86_64_isr_212
    .quad xiul_x86_64_isr_213
    .quad xiul_x86_64_isr_214
    .quad xiul_x86_64_isr_215
    .quad xiul_x86_64_isr_216
    .quad xiul_x86_64_isr_217
    .quad xiul_x86_64_isr_218
    .quad xiul_x86_64_isr_219
    .quad xiul_x86_64_isr_220
    .quad xiul_x86_64_isr_221
    .quad xiul_x86_64_isr_222
    .quad xiul_x86_64_isr_223
    .quad xiul_x86_64_isr_224
    .quad xiul_x86_64_isr_225
    .quad xiul_x86_64_isr_226
    .quad xiul_x86_64_isr_227
    .quad xiul_x86_64_isr_228
    .quad xiul_x86_64_isr_229
    .quad xiul_x86_64_isr_230
    .quad xiul_x86_64_isr_231
    .quad xiul_x86_64_isr_232
    .quad xiul_x86_64_isr_233
    .quad xiul_x86_64_isr_234
    .quad xiul_x86_64_isr_235
    .quad xiul_x86_64_isr_236
    .quad xiul_x86_64_isr_237
    .quad xiul_x86_64_isr_238
    .quad xiul_x86_64_isr_239
    .quad xiul_x86_64_isr_240
    .quad xiul_x86_64_isr_241
    .quad xiul_x86_64_isr_242
    .quad xiul_x86_64_isr_243
    .quad xiul_x86_64_isr_244
    .quad xiul_x86_64_isr_245
    .quad xiul_x86_64_isr_246
    .quad xiul_x86_64_isr_247
    .quad xiul_x86_64_isr_248
    .quad xiul_x86_64_isr_249
    .quad xiul_x86_64_isr_250
    .quad xiul_x86_64_isr_251
    .quad xiul_x86_64_isr_252
    .quad xiul_x86_64_isr_253
    .quad xiul_x86_64_isr_254
    .quad xiul_x86_64_isr_255

.section .note.GNU-stack,"",@progbits
