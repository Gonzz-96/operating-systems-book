import java.util.concurrent.*;

public class ThreadPoolExample {

    public static void main(String[] args) {
    
        int numTasks = Integer.parseInt(args[0].trim());

        ExecutorService pool = Executors.newCachedThreadPool();

        for (int i = 0; i < numTasks; i++) {
            final int finalI = i;
            pool.execute(() -> {
                System.out.println("Hello from task #" + finalI);
            });
        }

        pool.shutdown();
    }
}

