public class Lock {
    private static Lock lock = new Lock();

    public static void main(String[] args) throws Exception {
        Thread t = new Thread(new Runnable(){
			@Override
			public void run(){
				synchronized (lock){
					try{
						Thread.sleep(10000000L);
					}catch(Exception e){
						e.printStackTrace();
					}
				}
			}
		});
		t.start();
		for(int i=0;i<50;i++){
			new Thread(new Runnable(){
			@Override
			public void run(){
				synchronized (lock){
				    System.out.println("等");
				}
			}
		}).start();
		}
    }
}