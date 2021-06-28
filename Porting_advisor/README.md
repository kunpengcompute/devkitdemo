# Porting Advisor demo
鲲鹏代码迁移工具是一款可以简化客户应用迁移到基于鲲鹏916/920的服务器的过程的工具。工具仅支持Linux到kunpeng Linux的扫描和分析，可使用该工具分析可迁移性和迁移投入，也可使用该工具自动分析出需要修改的代码内容，并指导用户如何进行修改。
这个demo展示了如何使用鲲鹏代码迁移工具中的各个功能模块进行代码迁移，并指导用户如何修改扫描出的需要修改的代码行并给出修改建议。
\
\
testdemo文件夹包含code_migration,byte_check,asm_trans,weak_consistency文件夹。
- code_migration \
  该文件夹中包含code(优化前的示例代码)和code_mod(优化后的示例代码)，是对工具“源码迁移”功能的示例。
- byte_check \
  该文件夹中包含bytecheck(示例代码)，是对工具“增强功能-结构体字节对齐检查”功能的示例。
- asm_trans \
  该文件夹中包含all_asm(全汇编示例代码)、mult_inline_asm(嵌入式汇编多指令示例代码)和single_inline_asm(嵌入式汇编单指令示例代码)三个使用内嵌汇编的示例代码，是对工具“源码迁移”功能的示例。
- weak_consistency \
  该文件夹中包含atomic(使用c++ atomic的示例代码)、common(普通场景的示例代码)和lock(含锁保护的示例代码)，是对工具“增强功能-内存一致性检查”功能的示例。
\
\
具体使用流程说明书详见华为云鲲鹏开发套件-鲲鹏代码迁移工具 https://support.huaweicloud.com/kunpengdevps/kunpengdevps.html
