SECTIONS
{
    /* 确保通用的 .bss 段先被处理（包括普通变量） */
    .bss ALIGN(8) : {
        *(.bss)
        *(.bss.*)
        *(COMMON)
    }
    .app_buffer ALIGN(8) : {
        KEEP(*(.bss.$app_buffer))
    }
    /* 放置 Poincare 池，并记录地址 */
    .poincare_pool ALIGN(8) : {
        _poincare_pool_start = .;
        KEEP(*(.bss.$poincare_pool))
        _poincare_pool_end = .;
    }

    /* 放置 TreeStack，紧接其后 */
    .tree_stack ALIGN(8) : {
        _tree_stack_start = .;
        KEEP(*(.bss.$tree_stack))
        _tree_stack_end = .;
    }
}
INSERT AFTER .bss;