#include <iostream>

// 有问题的斐波那契函数类
class ProblematicFibonacci {
private:
    int _calculate(int n);
public:
    int calculate(int n);
};

// 优化后的斐波那契函数，使用迭代，避免重复计算
int ProblematicFibonacci::_calculate(int n) {
    if (n <= 1)
        return n;
    int a = 0, b = 1, c;
    for (int i = 2; i <= n; i++) {
        c = a + b;
        a = b;
        b = c;
    }
    return b;
}

// 调用私有计算函数
int ProblematicFibonacci::calculate(int n) {
    return this->_calculate(n);
}

// 封装 C 接口
extern "C" {
    // 创建对象
    ProblematicFibonacci* problematic_fibonacci_new() {
        return new ProblematicFibonacci;
    }

    // 调用计算方法
    int problematic_fibonacci_calculate(ProblematicFibonacci* t, int n) {
        return t->calculate(n);
    }

    // 释放对象
    void problematic_fibonacci_delete(ProblematicFibonacci* t) {
        delete t;
    }
}