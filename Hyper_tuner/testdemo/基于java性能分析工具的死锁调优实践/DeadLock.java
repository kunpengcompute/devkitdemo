public class DeadLock {
	private static final Integer lockOne = new Integer(1);
	private static final Integer lockTwo = new Integer(2);
	
	public static void main(String[] args){
		new Thread(()->{
			try{
				System.out.println("thread1 is running");
				sysnchronized (lockOne){
					System.out.println("thread1 get lock obj1");
					Thread.sleep(1000L);
					sysnchronized (lockTwo){
					    System.out.println("thread12 get lock obj2");
					    Thread.sleep(1000L);
				    }
				}
			} catch(InterruptedException e){
				e.printStackTrace();
			}
		}).start();
		new Thread(()->{
			try{
				System.out.println("thread2 is running");
				sysnchronized (lockTwo){
					System.out.println("thread2 get lock obj2");
					Thread.sleep(1000L);
					sysnchronized (lockOne){
					    System.out.println("thread2 get lock obj1");
					    Thread.sleep(1000L);
				    }
				}
			} catch(InterruptedException e){
				e.printStackTrace();
			}
		}).start();
	}
}