import java.util.concurrent.*;

public final class ForkJoinExample {

    private static final int SIZE = 10000;

    public static void main(String[] args) {

        ForkJoinPool pool = new ForkJoinPool();
        int[] array = new int[SIZE];

        SumTask task = new SumTask(0, SIZE - 1, array);
        int sum = pool.invoke(task);
    }
}

