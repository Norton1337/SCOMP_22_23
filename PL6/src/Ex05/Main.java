package src.Ex05;

import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class Main {

    public static void main(String[] args) {
        int[] vector = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        int numThreads = 5;
        int numPartitions = 10;

        ExecutorService executor = Executors.newFixedThreadPool(numThreads);

        int partitionSize = vector.length / numPartitions;
        int first = 0;
        int last = partitionSize;

        for (int i = 0; i < numPartitions; i++) {
            if (i == numPartitions - 1) {
                last = vector.length;
            }

            Runnable worker = new Sum(vector, first, last);
            executor.execute(worker);

            first = last;
            last += partitionSize;
        }

        executor.shutdown();
    }
}

