public class Lock {
    private static Lock lock = new Lock();

    public static void main(String[] args) throws Exception {
        while (true) {
            for (int i = 0; i < 10; i++) {
                new Thread(new Runnable() {
                    @Override
                    public void run() {
                        synchronized (lock) {  
                            try{
								Thread.sleep(1000L); 
							}catch(Exception e){
								e.printStackTrace();
							}					
                          
                        }
                    }
                }).start();
            }
            Thread.sleep(3000L);
        }
    }
}