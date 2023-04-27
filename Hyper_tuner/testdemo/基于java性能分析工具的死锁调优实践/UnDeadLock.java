public class UnDeadLock {
	private static final Integer lockOne = new Integer(1);
	private static final Integer lockTwo = new Integer(2);
	private static volatile Integer i = 1;
  
	public static void main(String[] args){
		new Thread(()->{
			try{
				System.out.println("thread1 is running");
        while(true){
          if(i % 2 == 1){
            synchronized (lockOne){
              System.out.println("thread1 get lock obj1");
              Thread.sleep(1000L);
              synchronized (lockTwo){
                  System.out.println("thread12 get lock obj2");
                  Thread.sleep(1000L);
                }
            }
            i++;
          }
        }
			} catch(InterruptedException e){
				e.printStackTrace();
			}
		}).start();
		new Thread(()->{
			try{
				System.out.println("thread2 is running");
        while(true){
          if(i % 2 == 0){
            synchronized (lockOne){
              System.out.println("thread1 get lock obj1");
              Thread.sleep(1000L);
              synchronized (lockTwo){
                  System.out.println("thread12 get lock obj2");
                  Thread.sleep(1000L);
                }
            }
            i++;
          }
        }
			} catch(InterruptedException e){
				e.printStackTrace();
			}
		}).start();
	}
}
