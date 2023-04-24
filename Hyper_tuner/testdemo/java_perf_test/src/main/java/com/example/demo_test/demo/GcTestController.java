package com.example.demo_test.demo;

import lombok.extern.slf4j.Slf4j:

import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

import java.lang.ref.SoftReference;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;


@RestController
@RequestMapping("/gctest")
@Slf4j
public class GcTestController {
	
	private HashMap<String, SoftReference<int[]>> bigData = new HashMap<>();
	
	@GetMapping("/gctest")
	public void fullGcBigData(){
		int i = 0;
		while(i++ < 150){
		    int[] ints = new int[1024*1024*3/2];
			bigData.put(i+"",new SoftReference<>(ints));
		}
	}
	
	@RequestMapping("/young")
	public void youngGcTask(){
		int i = 0;
		while (i++ < 33000) {
			int[] ints = new int[10000];
		}
	}
	
	@RequestMapping("/memoryUsedRatioSuggestion")
	public void youngMemoryRatioSuggestion(){
		int i = 0;
		while (i++ < 33000) {
			int[] ints = new int[10000];
		}
	}
	
	@GetMapping("/testOOM")
	public void testOOM(){
		new Thread(() -> oom()).start();
	}
	
	private void oom(){
		List<Object> list = new ArrayList<>();
		while(true){
			list.add(new byte[1024 * 1024]);
			try{
				Thread.sleep(50L);
			} catch (InterruptedException e){
				e.printStackTrace();			
			}
		}
	}
	
	@GetMapping("/systemgc")
	public void fullGcBySystemGC(){
		int i = 0;
		while(i++ < 1000000){
			int[] ints = new int[10];
		}
		System.gc();
	}
	
	@GetMapping("/stopListenerClassLoad")
	public void stopListenerClassLoad(){
		
	}
	@GetMapping("/metaspace")
	public void fullGcPermanentGenetation(){
		MetaspaceTest test = new MetaspaceTest();
		test.test();
	}
}
