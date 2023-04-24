package com.example.demo_test.demo;

public class TestMemoryLeakKo{  
      public static HashSet<List<Person>> hashSetPerson = new HashSet<>();
      
      public static void main(String[] args){
          boolean flag = true;
          Thread thread = new Thread(() ->
              while(flag) {
                  List<Person> list = new ArrayList<>(1000);
                  for (int )
              }
          )
      }
}
