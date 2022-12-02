package com.example.demo_test.demo;

public class MethodDemoOne{
	
	public MethodDemoOne() {
		
	}
	
	public static void test(int threadNum) throws InterruptedException {
		while(threadNum > 0){
			MethodDemoOne demoOne = new MethodDemoOne();
			demoOne.calculate(199999);
			Thread.sleep(1000L);
			--threadNum;
		}
	}
	
	void factor(int number) {
		long start = System.currentTimeMillis();
		
		for(int i = 2; i <= number; ++i) {
			if (this.faccc(i)) {
				
			}
		}
		
		long cost = System.currentTimeMillis() - start;
		System.out.println("cost time: " + cost);
	}
	
	boolean faccc(int i){
		boolean k = true;
		for(int n = 2; n < i; ++n){
			if(i % n == 0){
				k = false;
				break;
			}
		}
		return k;
	}
}