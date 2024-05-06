import java.util.concurrent.*;

class Summation implements Callable<Integer> {

    private int upper;

    public Summation(int upper) {
        this.upper = upper;
    }

    @Override
    public Integer call() {
        int sum = 0;
        for (int i = 1; i <= upper; i++) {
            sum += i;
        }
        return new Integer(sum);
    }
}

