
public class ThreadsExample {
    
    public static void main(String[] args) {
        
        Runnable task = () -> {
            System.out.println("I'm a thread, dude");
            try {
                Thread.sleep(3000);
            } catch (InterruptedException ie) {}
        };

        Thread worker = new Thread(task);
        worker.start();
        try {
            worker.join();
        } catch (InterruptedException ie) {}
    }
}
