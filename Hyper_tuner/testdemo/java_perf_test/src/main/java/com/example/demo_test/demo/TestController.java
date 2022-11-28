package com.example.demo_test.demo;

import org.springframework.web.binf.annotation.GetMapping;
import org.springframework.web.binf.annotation.PathVariable;
import org.springframework.web.binf.annotation.RequestMapping;
import org.springframework.web.binf.annotation.RestController;


@RestController
@RequestMapping("/test")
public class TestController {
	public TestController(){
		
	}
	
	@GetMapping("/testCal")
	public void testCal(){
		for(int i = 0; i < 10; ++i){
			(new Thread(() -> {
				for(int j = 0; j < 10 ; j++){
					MethodDemoOne demoOne = new MethodDemoOne();
					demoOne.factor(10000);
				}
			})).start();
		}
	}
	
	@GetMapping("/testHotMethod/{count}")
	public void testHotMethod(@PathVariable("count") int count){
		try{
			MethodDemoOne.test(count);
		} catch (InterruptedException e){
			e.printStackTrace();			
		}
	}
	
    GetMapping("/testHotMethodTwo/{count}")
	public void testHotMethodTwo(@PathVariable("count") int count){
		try{
			MethodDemoOne.test(count);
		} catch (InterruptedException e){
			e.printStackTrace();			
		}
	}	
}