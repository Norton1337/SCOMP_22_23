package src.Ex06;

import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.*;

public class Main {

    public static void main(String[] args) {
        int[] vector = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        int numThreads = 5;
        int numPartitions = 10;

        ExecutorService executor = Executors.newFixedThreadPool(numThreads);
        List<Future<Integer>> futures = new ArrayList<>();

        int partitionSize = vector.length / numPartitions;
        int startIndex = 0;
        int endIndex = partitionSize;

        for (int i = 0; i < numPartitions; i++) {
            if (i == numPartitions - 1) {
                endIndex = vector.length;
            }

            Callable<Integer> worker = new Sum(vector, startIndex, endIndex);
            Future<Integer> future = executor.submit(worker);
            futures.add(future);

            startIndex = endIndex;
            endIndex += partitionSize;
        }

        executor.shutdown();

        int sum = 0;
        for (Future<Integer> future : futures) {
            try {
                sum += future.get();
            } catch (InterruptedException | ExecutionException e) {
                e.printStackTrace();
            }
        }

        System.out.println("Sum: " + sum);
    }
}

