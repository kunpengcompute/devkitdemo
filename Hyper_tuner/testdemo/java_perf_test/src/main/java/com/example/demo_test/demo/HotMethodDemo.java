package com.example.demo_test.demo;

public class HotMethodDemo{
	public HotMethodDemo() {
		
	}
	
	public static void test(int threadNum) throws InterruptedException {
		while(threadNum > 0){
			HotMethodDemo demoOne = new HotMethodDemo();
			demoOne.calculate(199999);
			Thread.sleep(1000L);
			--threadNum;
		}
	}
	
	void calculate(int number) {
		long start = System.currentTimeMillis();
		
		for(int i = 2; i <= number; ++i) {
			if (this.doCalculate(i)) {
				
			}
		}
		
		long cost = System.currentTimeMillis() - start;
		System.out.println("cost time: " + cost);
	}
	
	boolean doCalculate(int i){
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