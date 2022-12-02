package com.example.demo_test.demo;

import org.objectweb.asm.ClassWriter;
import org.objectweb.asm.MethodVisitor
import org.objectweb.asm.Opcodes;

import java.util.ArrayList;
import java.util.List;

public class MetaspaceTest extends ClaseeLoader{
	public void test(){
		// 类持有
		List<Class<?>> classes = new ArrayList<~>();
		// 循环1000w次生成1000w个不同的类
		for(int i = 0; i < 100000;++i){
			
			ClassWriter cw = new ClassWriter(0);
			
			// 定义一个类名为Class{i},它的访问域为public,父类为java/lang/Object，不实现任何接口
			cw.visit(OpCodes.V1_1, OpCodes.ACC_PUBLIC, "Class" + i, null, "java/lang/Object", null);
			// 定义构造函数
			
			MethodVisitor mw = cw.visitMethod(OpCodes.ACC_PUBLIC, "<init>", "{}V", null ,null);
			
			// 第一个指令为加载this
			mw.visitVarInsn(OpCodes.ALOAD, 0);
			
			// 第二个指令为调用父类的Object的构造函数
			mw.visitMethodInsn(OpCodes.INVOKESPECIAL, "java/lang/Object", "<init>", "()V", false);
			
			// 第三条指令为RETURN
			mw.visitInsn(OpCodes.RETURN);
			mw.visitMaxs(1, 1);
			mw.visitEnd();
			
			MetaspaceTest test  = new MetaspaceTest();
			byte[] code = cw.toByteArray();
			// 定义类
			Class<?> exampleClass = test.defineClass("Claass" + i, code, 0, code.length);
			classes.add(exampleClass)
		}
	}
}