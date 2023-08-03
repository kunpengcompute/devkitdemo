package io.jenkins.plugins.devkit.webserver;

public enum SrvType {
    running_mode,
    byte_alignment,
    cacheline_alignment,
    acceleration,   //构建亲和
    weakconsistency, //内存一致性
    vecanalyzer //向量化检查

}
