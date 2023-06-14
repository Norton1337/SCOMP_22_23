package src.Ex06;

import java.util.concurrent.Callable;

record Sum (int[] vector, int first, int last) implements Callable<Integer> {
    @Override
    public Integer call() {
        int sum = 0;
        for (int i = first; i < last; i++) {
            sum += vector[i];
        }
        return sum;
    }
}
