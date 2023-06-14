package src.Ex05;

record Sum (int[] vector, int first, int last) implements Runnable {

    @Override
    public void run() {
        int sum = 0;
        for (int i = first; i < last; i++) {
            sum += vector[i];
        }
        System.out.println("Sum from index " + first + " to " + last + ": " + sum);
    }
}
